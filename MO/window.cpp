#include "window.h"


namespace MO {
    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        switch(uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }


    HWND ShowMainWindow(HINSTANCE hInstance, INT nCmdShow) {
        WNDCLASS wc{};
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = CLASS_NAME;
        if(RegisterClass(&wc) == 0) {
            fprintf_s(stderr, "[ERROR] RegisterClass() errno %lu\n", GetLastError());
            return nullptr;
        }

        HWND hwnd = CreateWindowEx(0, wc.lpszClassName, WINDOW_NAME, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                                   CW_USEDEFAULT, CW_USEDEFAULT, 320, 180,
                                   nullptr, nullptr, hInstance, nullptr);
        if(hwnd == nullptr) {
            fprintf_s(stderr, "[ERROR] CreateWindowEx() errno %lu\n", GetLastError());
            return nullptr;
        }

        ShowWindow(hwnd, nCmdShow);

        return hwnd;
    }


    LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam) {
        if(nCode == HC_ACTION) {
            if(wParam == WM_KEYUP) {
                auto* kbdll = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
                auto hwnd = FindWindow(CLASS_NAME, WINDOW_NAME);
                PostMessage(hwnd, WM_USER, kbdll->vkCode, 0); // send as WPARAM
            }
        }
        return CallNextHookEx(nullptr, nCode, wParam, lParam);
    }


    UniqueHandle<HHOOK> CreateKbllHook() {
        auto hhook = SetWindowsHookEx(WH_KEYBOARD_LL, HookProc, nullptr, 0);
        if(hhook == nullptr)
            fprintf_s(stderr, "[ERROR] SetWindowsHookEx() errno %lu\n", GetLastError());
        return UniqueHandle<HHOOK>(hhook, [](auto hhk) { UnhookWindowsHookEx(hhk); });
    }


    bool MoveToFront(HWND hwnd) {
        if(hwnd == GetForegroundWindow())
            return true;

        if(SetForegroundWindow(hwnd) == FALSE) {
            if(GetLastError() == ERROR_INVALID_WINDOW_HANDLE) {
                fprintf_s(stderr, "[ERROR] SetForegroundWindow() target window closed.\n");
                return false;
            }
        }
        ShowWindow(hwnd, SW_SHOWDEFAULT);

        auto fgHwnd = GetForegroundWindow();
        if(hwnd == fgHwnd)
            return true;

        auto thread = GetWindowThreadProcessId(hwnd, nullptr);
        auto fgThread = GetWindowThreadProcessId(fgHwnd, nullptr);

        AttachThreadInput(fgThread, thread, TRUE);
        SetForegroundWindow(hwnd);
        ShowWindow(hwnd, SW_SHOWDEFAULT);
        BringWindowToTop(hwnd);
        AttachThreadInput(fgThread, thread, FALSE);

        return hwnd == GetForegroundWindow();
    }
}
