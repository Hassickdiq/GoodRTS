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
