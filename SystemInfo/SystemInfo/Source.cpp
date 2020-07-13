// MathLibrary.cpp : Defines the exported functions for the DLL.
#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <Windows.h>
#include <fstream>
#include <string>
#include <wininet.h>

#include "SystemInfo.h"

#pragma comment( lib,"Wininet.lib")

#define INFO_BUFFER_SIZE 32767

// DLL internal state variables:
static unsigned long long previous_;  // Previous value, if any
static unsigned long long current_;   // Current sequence value
static unsigned index_;               // Current seq. position

void postData(const std::wstring data)
{
    HINTERNET Initialize, Connection, hRequest;
    DWORD dwBytes;
    LPCWSTR accept[2] = { L"*/*", NULL };
    LPCWSTR hdrs = L"Content-Type: text/html; charset=utf-8";
    LPVOID frmdata = (LPVOID)"paste_code=test";
    const char* testfrmdata = "paste_code=test";


    Initialize = InternetOpen(L"https generic", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (Initialize != NULL)
    {   
        // open HTTP session
        Connection = InternetConnect(Initialize, L"127.0.0.1", 5000, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
        if (Connection != NULL)
        {
            hRequest = HttpOpenRequest(Connection, L"POST", L"/data", NULL, NULL, accept, 0, 0);
            
            if (hRequest != NULL)
            {
                bool send = HttpSendRequest(hRequest, hdrs, wcslen(hdrs), frmdata, 15);

                if (!send) {
                    OutputDebugString(L"HttpSendRequest failed");
                    DWORD result = GetLastError();
                    OutputDebugString(std::to_wstring(result).data());
                }
            }
            else {
                OutputDebugString(L"HttpOpenRequest failed");
            }
        }
        else {
            OutputDebugString(L"InternetConnect failed");
        }
    }
    else {
        OutputDebugString(L"InternetOpen failed");
    }
    

    
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

    return systemInfo;
}

void getSystemInfo()
{
    const std::wstring systemInfo = getSystemInfoData();
    postData(systemInfo);
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

    GetWindowsDirectoryW(const_cast<wchar_t*>(windowsDirectory.data()), INFO_BUFFER_SIZE);

    return windowsDirectory;
}

const MEMORYSTATUSEX wrapGetSystemMemoryStatus()
{
    MEMORYSTATUSEX memoryStatus;
    memoryStatus.dwLength = sizeof(memoryStatus);
    GlobalMemoryStatusEx(&memoryStatus);

    return memoryStatus;
}
