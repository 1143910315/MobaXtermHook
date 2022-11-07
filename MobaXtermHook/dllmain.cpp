// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <tchar.h>
#include <string>
#include <fstream>
#include "detours/detours.h"
#pragma comment(lib, "detours/detours.lib")
using namespace std;

#ifdef _UNICODE
#define tstring wstring
#define tfstream wfstream
#else
#define tstring string
#define tfstream fstream
#endif // _UNICODE


static BOOL(WINAPI* TShowWindow)(HWND hWnd, int nCmdShow) = ShowWindow;
static HWND(WINAPI* TCreateWindowExA)(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) = CreateWindowExA;
static tfstream f;
static string password;

void sendText(HWND hWnd, const char* text) {
    char c;
    while (c = *text++) {
        PostMessageA(hWnd, 258, c, 0);
    }
}
void mouseMessage(HWND hWnd = GetDesktopWindow(), int x = 2, int y = 2, int key = 0, int control = 0) {
    int point = x + y * 65536;
    if (key == 4) {
        PostMessageA(hWnd, 522, 7864336, point + 25100873);
        return;
    }
    if (key == 5) {
        PostMessageA(hWnd, 522, -7864304, point + 25100873);
        return;
    }
    PostMessageA(hWnd, 512, 2, point);
    switch (key)
    {
    case 0:
    case 1: {
        switch (control) {
        case 0:
        case 1: {
            PostMessageA(hWnd, 513, 1, point);// 左键按下
            PostMessageA(hWnd, 514, 0, point);// 左键放开
            break;
        }
        case 2: {
            PostMessageA(hWnd, 513, 1, point);
            PostMessageA(hWnd, 514, 0, point);
            PostMessageA(hWnd, 515, 0, point);// 双击左键
            break;
        }
        case 3: {
            PostMessageA(hWnd, 513, 1, point);
            break;
        }
        case 4: {
            PostMessageA(hWnd, 514, 0, point);
            break;
        }
        default:
            break;
        }
        break;
    }
    case 2: {
        switch (control) {
        case 0:
        case 1: {
            PostMessageA(hWnd, 516, 2, point);// 右键按下
            PostMessageA(hWnd, 517, 2, point);// 右键放开
            break;
        }
        case 2: {
            PostMessageA(hWnd, 516, 2, point);
            PostMessageA(hWnd, 517, 2, point);
            PostMessageA(hWnd, 518, 0, point);// 双击右键
            break;
        }
        case 3: {
            PostMessageA(hWnd, 516, 2, point);
            break;
        }
        case 4: {
            PostMessageA(hWnd, 517, 2, point);
            break;
        }
        default:
            break;
        }
        PostMessageA(hWnd, 123, 132782, point + 25100873);// 右键放开
        break;
    }
    case 3: {
        switch (control) {
        case 0:
        case 1: {
            PostMessageA(hWnd, 519, 16, point);// 中键按下
            PostMessageA(hWnd, 520, 0, point);// 中键放开
            break;
        }
        case 2: {
            PostMessageA(hWnd, 519, 16, point);
            PostMessageA(hWnd, 520, 0, point);
            PostMessageA(hWnd, 521, 0, point);// 双击中键
            break;
        }
        case 3: {
            PostMessageA(hWnd, 519, 16, point);
            break;
        }
        case 4: {
            PostMessageA(hWnd, 520, 0, point);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }

}
BOOL WINAPI MyDeleteFileA(HWND hWnd, int nCmdShow) {
    int textLength = max(GetWindowTextLength(hWnd), 256);
    TCHAR* str = new TCHAR[textLength + 1];
    GetWindowText(hWnd, str, textLength + 1);
    tstring title = tstring(str);
    GetClassName(hWnd, str, textLength + 1);
    tstring className = tstring(str);
    delete[]str;
    //f << "窗口句柄：" << hWnd << "；窗口显示模式：" << nCmdShow << "；窗口标题：" << title << "；窗口类名：" << className << endl;
    if (title == "MobaXterm主密码") {
        HWND child = FindWindowExA(hWnd, NULL, "TsEdit", nullptr);
        if (child != NULL) {
            //PostMessageA(child, 12, 0, (LPARAM)"");
            sendText(child, password.c_str());
            child = FindWindowExA(hWnd, child, "TsEdit", nullptr);
            if (child != NULL) {
                //PostMessageA(child, 12, 0, (LPARAM)"");
                sendText(child, password.c_str());
                child = FindWindowExA(hWnd, NULL, "TsBitBtn", "好的");
                if (child != NULL) {
                    mouseMessage(child);
                    nCmdShow = 0;
                }
            }
        }
    }
    if (className == "TFormInputDialog" && title == "MobaXterm") {
        HWND child = FindWindowExA(hWnd, NULL, "TsEdit", nullptr);
        if (child != NULL && FindWindowExA(hWnd, child, "TsEdit", nullptr) == NULL && (FindWindowExA(hWnd, NULL, "TsCheckBox", "在这台电脑上记住我") != NULL || FindWindowExA(hWnd, NULL, "TsCheckBox", "显示密码") != NULL)) {
            sendText(child, password.c_str());
            child = FindWindowExA(hWnd, NULL, "TsBitBtn", "好的");
            if (child != NULL) {
                mouseMessage(child);
                nCmdShow = 0;
            }
        }
    }
    return TShowWindow(hWnd, nCmdShow);
}
HWND WINAPI MyCreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) {
    f << "dwExStyle：" << dwExStyle << "：lpClassName：" << lpClassName << "；lpWindowName：" << lpWindowName << "；dwStyle：" << dwStyle
        << "；X：" << X << "；Y：" << Y << "；nWidth：" << nWidth << "；nHeight：" << nHeight << "；hWndParent：" << hWndParent
        << "；hMenu：" << hMenu << "；hInstance：" << hInstance << "；lpParam：" << lpParam << endl;
    return TCreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}
void SetHook() {
    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)TShowWindow, MyDeleteFileA);
    //DetourAttach(&(PVOID&)TCreateWindowExA, MyCreateWindowExA);
    DetourTransactionCommit();
    //f.open("convex.txt", ios::out | ios::app);
    f.open("password.txt", ios::in);
    f >> password;
}
void UnHook() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)TShowWindow, MyDeleteFileA);
    //DetourDetach(&(PVOID&)TCreateWindowExA, MyCreateWindowExA);
    DetourTransactionCommit();
}
__declspec(dllexport) void ExportFunc(void) {
}
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:SetHook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

