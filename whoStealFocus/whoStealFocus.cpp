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

// ��������ʱ�Ĵ�����Ϣ
typedef struct WindowInfoEnum
{
    DWORD dwProcessID;
    HWND hWnd;
} *lpWindowInfoEnum;

extern "C" void __declspec(dllimport) __stdcall SetHook(HWND hwnd, const char* logPath);
HWND GetHWNDByExeFileName(char* exeFileName);
BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM lParam);

// ͨ����������ȡ HWND
HWND GetHWNDByExeFileName(char* exeFileName)
{
    // ���̿���
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
    if (INVALID_HANDLE_VALUE == hSnap)
    {
        return nullptr;
    }

    // �����ȽϽ���������¼ PID
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

    // �������ڣ��Ƚ� PID ����¼ HWND
    WindowInfoEnum wi;
    wi.dwProcessID = pid;
    wi.hWnd = nullptr;
    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&wi));

    HWND hwnd = wi.hWnd;
    // �ҵ����㴰�ھ��
    while (GetParent(hwnd) != nullptr)
    {
        hwnd = GetParent(hwnd);
    }

    return hwnd;
}

// �������ڵĻص�����
BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM lParam)
{
    DWORD pid;
    // ͨ�� HWND ȡ PID
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
