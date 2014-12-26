// 
// Module			: whoStealFocus.cpp
// Author			: bss
// Project			: WhoStealFocus
// State			: 
// Creation Date	: 2014-12-26
// Last Modification: ???
// Description		: main
// 

#include <stdio.h>
#include <Windows.h>
#include <TlHelp32.h>
#include "settings.h"

#pragma comment(lib, "hookLostFocus.lib")

// 遍历窗口时的窗口信息
typedef struct WindowInfoEnum
{
    DWORD dwProcessID;
    HWND hWnd;
} *lpWindowInfoEnum;

extern "C" void __declspec(dllimport) __stdcall SetHook(HWND hwnd, const char* logPath);
HWND GetHWNDByExeFileName(char* exeFileName);
BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM lParam);

// 通过进程名获取 HWND
HWND GetHWNDByExeFileName(char* exeFileName)
{
    // 进程快照
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
    if (INVALID_HANDLE_VALUE == hSnap)
    {
        return nullptr;
    }

    // 遍历比较进程名，记录 PID
    DWORD pid = 0;
    PROCESSENTRY32 ProcessEntry;
    ProcessEntry.dwSize = sizeof(ProcessEntry);
    BOOL isCopied = Process32First(hSnap, &ProcessEntry);
    while (isCopied)
    {
        if (0 == strcmp(ProcessEntry.szExeFile, exeFileName))
        {
            pid = ProcessEntry.th32ProcessID;
            break;
        }
        isCopied = Process32Next(hSnap, &ProcessEntry);
    }
    if (0 == pid)
    {
        return nullptr;
    }

    // 遍历窗口，比较 PID 并记录 HWND
    WindowInfoEnum wi;
    wi.dwProcessID = pid;
    wi.hWnd = nullptr;
    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&wi));

    HWND hwnd = wi.hWnd;
    // 找到顶层窗口句柄
    while (GetParent(hwnd) != nullptr)
    {
        hwnd = GetParent(hwnd);
    }

    return hwnd;
}

// 遍历窗口的回调函数
BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM lParam)
{
    DWORD pid;
    // 通过 HWND 取 PID
    GetWindowThreadProcessId(hwnd, &pid);

    lpWindowInfoEnum lpWi = reinterpret_cast<lpWindowInfoEnum>(lParam);
    if (pid == lpWi->dwProcessID)
    {
        lpWi->hWnd = hwnd;
        return FALSE;
    }

    return TRUE;
}

// main
int main()
{
    printf("Press Ctrl-C to quit.\n");
    Settings settings = {};
    LoadSettings("settings.ini", &settings);

    HWND hTarget = GetHWNDByExeFileName(settings.targetName);
    SetHook(hTarget, settings.logPath);

    while ('q' != getchar())
    {
        continue;
    }
    return 0;
}
