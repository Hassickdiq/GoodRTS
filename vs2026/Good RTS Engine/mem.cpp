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

#include "mem.h"

#include <sstream>
#include <iomanip>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define NODRAWTEXT

#include <windows.h>
#include <psapi.h>

std::string GetMemoryUsage() {
    PROCESS_MEMORY_COUNTERS pmc = {};
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        double memoryMB = pmc.WorkingSetSize / (1024.0 * 1024.0);

        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << memoryMB << " MB";
        return ss.str();
    }
    return "Hata";
}
