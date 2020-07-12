// MathLibrary.cpp : Defines the exported functions for the DLL.
#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <Windows.h>
#include <utility>
#include <limits.h>
#include <fstream>
#include <iostream>
#include <string>
#include "SystemInfo.h"

#define INFO_BUFFER_SIZE 32767

// DLL internal state variables:
static unsigned long long previous_;  // Previous value, if any
static unsigned long long current_;   // Current sequence value
static unsigned index_;               // Current seq. position

void writeToFile(const std::string path, const std::wstring text)
{
    std::wofstream fs(path);
    OutputDebugString(L"writing to file");
    if (!fs)
    {
        OutputDebugString(L"Cannot open the output file.");
    }
    fs << text;
    fs.close();
}

const std::wstring getSystemInfoData()
{
    std::wstring systemInfo = L"";

    std::wstring computerName = wrapGetComputerName();
    systemInfo += L"HOSTNAME: " + computerName + L"\n";

    MEMORYSTATUSEX memoryStatus = wrapGetSystemMemoryStatus();
    systemInfo += L"Total Physical Memory: " + std::to_wstring(memoryStatus.ullTotalPhys / 1024 / 1024) + L" MB \n";

    systemInfo += L"Available Physical Memory: " + std::to_wstring(memoryStatus.ullAvailPhys / 1024 / 1024) + L" MB \n";

    std::wstring windowsDriectory = wrapGetWindowsDirectoryName();
    systemInfo += L"Windows Directory: " + windowsDriectory + L"\n";

    /*
    systemInfo += "OS Name : Microsoft Windows 10 Pro\n";
    systemInfo += "OS Version : 10.0.18362 N / A Build 18362\n";
    systemInfo += "OS Manufacturer : Microsoft Corporation\n";
    systemInfo += "OS Configuration : Standalone Workstation\n";
    systemInfo += "OS Build Type : Multiprocessor Free"; */
    return systemInfo;
}

void getSystemInfo()
{
    const std::wstring systemInfo = getSystemInfoData();
    writeToFile("C:\\a.txt", systemInfo);
}

const std::wstring wrapGetComputerName() 
{
    std::wstring computerName(MAX_COMPUTERNAME_LENGTH + 1, NULL);
    DWORD size = MAX_COMPUTERNAME_LENGTH + 1;

    GetComputerNameW(const_cast<wchar_t*>(computerName.data()), &size);
    computerName.resize(size);

    return computerName;
}

const std::wstring wrapGetWindowsDirectoryName()
{
    std::wstring windowsDirectory(INFO_BUFFER_SIZE, NULL);
    //UINT size = MAX_PATH;

    GetWindowsDirectoryW(const_cast<wchar_t*>(windowsDirectory.data()), INFO_BUFFER_SIZE);
    //windowsDirectory.resize();

    return windowsDirectory;
}

const MEMORYSTATUSEX wrapGetSystemMemoryStatus()
{
    MEMORYSTATUSEX memoryStatus;
    memoryStatus.dwLength = sizeof(memoryStatus);
    GlobalMemoryStatusEx(&memoryStatus);
    //windowsDirectory.resize();

    return memoryStatus;
}
