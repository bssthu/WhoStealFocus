// 
// Module			: hookLostFocus.cpp
// Author			: bss
// Project			: WhoStealFocus
// State			: 
// Creation Date	: 2014-12-26
// Last Modification: ???
// Description		: 
// 

#include <stdio.h>
#include <Windows.h>
#include <time.h>

static const int LEN = 2048;
HHOOK g_hookLF = nullptr;

// È¡Ê±¼ä×Ö·û´®
void FormatTimeString(char* timeStr)
{
    time_t now = time(nullptr);
    tm timeMsg;
    localtime_s(&timeMsg, &now);
    strftime(timeStr, sizeof(timeMsg), "%Y-%m-%d %H:%M:%S", &timeMsg);
}

LRESULT CALLBACK HookProcLF(int nCode, WPARAM wParam, LPARAM lParam)
{
    LPCWPSTRUCT pcwp = (LPCWPSTRUCT)lParam;

    if (pcwp->message == WM_KILLFOCUS || pcwp->message == WM_SETFOCUS)
    {
        char timeStr[128] = "";
        FormatTimeString(timeStr);
        char className[LEN] = "";
        char classNameWP[LEN] = "null";
        GetClassName(pcwp->hwnd, className, LEN);
        if (pcwp->wParam != 0)
        {
            GetClassName((HWND)pcwp->wParam, classNameWP, LEN);
        }
        FILE* fp = fopen("R:\\hook.log", "at+");
        if (pcwp->message == WM_KILLFOCUS)
        {
            fprintf(fp, "%s %s -> %s\n", timeStr, className, classNameWP);
        }
        else if (pcwp->message == WM_SETFOCUS)
        {
            fprintf(fp, "%s %s <- %s\n", timeStr, className, classNameWP);
        }
        fclose(fp);
    }

    return CallNextHookEx(g_hookLF, nCode, wParam, lParam);
}

extern "C" void __declspec(dllexport) __stdcall SetHook(HWND /*hwnd*/)
{
    g_hookLF = SetWindowsHookEx(WH_CALLWNDPROC, HookProcLF, GetModuleHandle("HookLostFocus"), 0);
    FILE* fp = fopen("R:\\hook.log", "at+");
    fprintf(fp, "init\n");
    fclose(fp);
}
