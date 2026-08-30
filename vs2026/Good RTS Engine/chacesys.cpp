/*
 * GoodRTS
 * Copyright (C) Copyright (C) 2026 Batuhan Demir (Hassickdiq)
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "chacesys.hpp"

using namespace std;

void GameCache::UnloadAll() {
    for (auto& pair : Textures) {
        UnloadTexture(pair.second);
    }
    Textures.clear();

    for (auto& pair : Sounds) {
        UnloadSound(pair.second);
    }
    Sounds.clear();

    for (auto& pair : Animations) {
        pair.second.frames.clear();
    }
    Animations.clear();

    TpTiles.clear();
    TpEntitys.clear();
}

void GameCache::Load(std::string path, lua_State* L) {
    if (Files[path]) return;

    int len = path.length();
    string type = path.substr(path.find('.') + 1);
    for (char& c : type) {
        c = toupper(c);
    }

    cout << "[" << this << "]" << "[" << type << "]" << " Load: " << path << endl;

    if (type == "PNG") {
        if (FileExists(path.c_str())) {
            Textures[path] = LoadTexture(path.c_str());
        }
        else
            cout << "The file could not be opened. " << path << endl;
    }
    else if (type == "WAV") {
        if (FileExists(path.c_str())) {
            Sounds[path] = LoadSound(path.c_str());
        }
        else
            cout << "The file could not be opened. " << path << endl;
    }
    else if (type == "CHC") {
        CHCInterpreter(path, L);
    }
    else if (type == "PSA") {
        PSAInterpreter(path);
    }
    else if (type == "ENT") {
        ENTInterpreter(path);
    }
    else if (type == "TLP") {
        TLPInterpreter(path);
    }
    else if (type == "LUA") {
        if (luaL_dofile(L, path.c_str()) != LUA_OK) {
            std::cout << "Lua load error (" << path << "): " << lua_tostring(L, -1) << std::endl;
            lua_pop(L, 1);
        }
    }
    else {
        cout << "Unknown type. " << path << endl;
        return;
    }

    Files[path] = true;
}

vector<string> FileToLineBuffer(string name) {
    vector<string> buffer;
    std::string line;

    std::ifstream file(name);
    if (!file.is_open()) {
        std::cerr << "The file could not be opened." << std::endl;
        return buffer;
    }

    while (getline(file, line)) {
        buffer.push_back(line);
    }

    return buffer;
}

vector<string> LoadPaths(vector<string>* buffer, int* i) {
    vector<string> paths;
    if (*i < buffer->size() && (*buffer)[(*i)] == "*paths") {
        (*i)++; // "*paths"

        while ((*buffer)[(*i)] != "*end" && *i < buffer->size()) {
            paths.push_back((*buffer)[(*i)]); // paths
            (*i)++;
        }
        if (*i < buffer->size() && (*buffer)[*i] == "*end") {
            (*i)++; // "*end"
        }
        return paths;
    }
    return paths;
}

string GetValue(string key, vector<string>* buffer, int* i) {
    string value = "";
    if ((*buffer)[(*i)] == key) {
        (*i)++;
        value = (*buffer)[(*i)];
        (*i)++;
    }
    else
        cout << "The \"" << key << "\" key was not found." << endl;
    return value;
}

void GameCache::TLPInterpreter(std::string path) {
    vector<string> buffer = FileToLineBuffer(path);

    int i = 0;
    int len = buffer.size() - 1;
    bool error = false;

    string value;
    string name;
    TpTile newTpTile;

    while (i < len - 1) {
        if (buffer[i] == "*tile") {
            error = false;
            for (size_t j = 0; j < 5; j++) {
                if (i < len) i++;
                else { error = true; break; }

                value = buffer[i];

                if (j == 0) // name
                    name = value;
                else if (j == 1) // anim
                    newTpTile.animPath = value;
                else if (j == 2) // X
                    try { newTpTile.offX = static_cast<int8_t>(stoi(value)); }
                    catch (const std::invalid_argument& e) { cout << "This is not a number." << endl; error = true; }
                else if (j == 3) // Y
                    try { newTpTile.offY = static_cast<int8_t>(stoi(value)); }
                    catch (const std::invalid_argument& e) { cout << "This is not a number." << endl; error = true; }
                else if (j == 4) // *end
                    {i++; break;}
                else { error = true; break; }
            }
        }
        else {
            cout << "The \"*tile\" array was not found." << endl;
            return;
        }

        TpTiles[name] = newTpTile;
    }
}

void GameCache::ENTInterpreter(std::string path) {
    vector<string> buffer = FileToLineBuffer(path);
    vector<string>* buffer_ptr = &buffer;

    int i = 0;
    bool error = false;

    string value = "";
    string name = "";
    TpEntity newTpEntity;

    value = GetValue("name:", buffer_ptr, &i);
    if (!value.empty())
        name = value;
    else return;

    value = GetValue("luaPrefix:", buffer_ptr, &i);
    if (!value.empty())
        newTpEntity.luaPrefix = value;
    else return;

    vector<string> paths = LoadPaths(buffer_ptr, &i);
    for (string path : paths) {
        newTpEntity.animPath.push_back(path);
    }
    paths = LoadPaths(buffer_ptr, &i);
    for (string path : paths) {
        newTpEntity.soundPath.push_back(path);
    }

    value = GetValue("size:", buffer_ptr, &i);
    if (!value.empty())
        try { newTpEntity.size = stof(value); }
        catch (const std::invalid_argument& e) { cout << "This is not a number." << endl; error = true; }
    else return;

    value = GetValue("isAnchor:", buffer_ptr, &i);
    if (!value.empty())
        try { newTpEntity.isAnchor = (stoi((value)) > 0); }
        catch (const std::invalid_argument& e) { cout << "This is not a bool." << endl; error = true; }
    else return;

    value = GetValue("minDamage:", buffer_ptr, &i);
    if (!value.empty())
        try { newTpEntity.minDamage = stof(value); }
    catch (const std::invalid_argument& e) { cout << "This is not a number." << endl; error = true; }
    else return;

    value = GetValue("maxDamage:", buffer_ptr, &i);
    if (!value.empty())
        try { newTpEntity.maxDamage = stof(value); }
    catch (const std::invalid_argument& e) { cout << "This is not a number." << endl; error = true; }
    else return;

    value = GetValue("attackRadius:", buffer_ptr, &i);
    if (!value.empty())
        try { newTpEntity.attackRadius = stof(value); }
    catch (const std::invalid_argument& e) { cout << "This is not a number." << endl; error = true; }
    else return;

    value = GetValue("maxHitPoint:", buffer_ptr, &i);
    if (!value.empty())
        try { newTpEntity.maxHitPoint = stoi(value); }
        catch (const std::invalid_argument& e) { cout << "This is not a number." << endl; error = true; }
    else return;

    value = GetValue("eClass:", buffer_ptr, &i);
    if (!value.empty())
        try { newTpEntity.eClass = stoi(value); }
    catch (const std::invalid_argument& e) { cout << "This is not a number." << endl; error = true; }
    else return;

    if (!error){
        newTpEntity.isThere = true;
        TpEntitys[name] = newTpEntity;
    }
}

void GameCache::PSAInterpreter(string path) {
    vector<string> buffer = FileToLineBuffer(path);
    vector<string>* buffer_ptr = &buffer;

    int i = 0;
    bool error = false;

    string value = "";
    string name = "";
    Anim2D newAnim2D;

    vector<string> paths = LoadPaths(buffer_ptr, &i);
    for (string path : paths) {
        newAnim2D.frames.push_back(&Textures[path]);
    }

    value = GetValue("fps:", buffer_ptr, &i);
    if (!value.empty())
        try { newAnim2D.fps = stod(value); }
        catch (const std::invalid_argument& e) { cout << "This is not a number." << endl; error = true; }
    else return;

    value = GetValue("index:", buffer_ptr, &i);
    if (!value.empty())
        try { newAnim2D.index = stoi(value); }
    catch (const std::invalid_argument& e) { cout << "This is not a number." << endl; error = true; }
    else return;

    value = GetValue("name:", buffer_ptr, &i);
    if (!value.empty())
        name = value;
    else return;

    if (!error)
        Animations[name] = newAnim2D;
}

void GameCache::CHCInterpreter(string path, lua_State* L) {
    vector<string> buffer = FileToLineBuffer(path);
    vector<string>* buffer_ptr = &buffer;

    int i = 0;

    vector<string> paths = LoadPaths(buffer_ptr, &i);
    for (string path : paths) {
        Load(path, L);
    }
}
