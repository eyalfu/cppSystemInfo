// SYSEMINFO_API.h - Contains declarations of math functions
#pragma once

extern "C" __declspec (dllimport) void  writeToFile(const std::string path, const std::wstring text);

extern "C" __declspec (dllimport) const std::wstring getSystemInfoData();

extern "C" __declspec (dllimport) const std::wstring wrapGetComputerName();

extern "C" __declspec (dllimport) const std::wstring wrapGetWindowsDirectoryName();

extern "C" __declspec (dllimport) const MEMORYSTATUSEX wrapGetSystemMemoryStatus();

extern "C" __declspec (dllexport) void __cdecl getSystemInfo();