#include "MO/app.h"
#include "MO/capture.h"
#include "MO/console.h"
#include "MO/ocr.h"
#include "MO/util.h"
#include "MO/window.h"

#include <Windows.h>

#include <chrono>
#include <thread>


void HandleKey(const MO::ScreenCapturer& capturer, const MO::OcrWrapper& ocre, const DWORD vkCode) {
    fprintf_s(stdout, "[LOG] vkCode %lu\n", vkCode);
    // use left control as hot key
    if(vkCode != VK_LCONTROL)
        return;

    auto word = MO::GetWord(capturer, ocre);
    if(word.length() == 0)
        return;

    fprintf_s(stdout, "[LOG] OCR: %ls\n", word.c_str());
    if(word.length() > 32) // if too long
        return;

    auto mdictHwnd = FindWindow(nullptr, MO::MDICT);
    if(mdictHwnd == nullptr) {
        fprintf_s(stderr, "[WARN] MDict window not found.\n");
        return;
    }

    MO::MoveToFront(mdictHwnd);

    MO::SendWord(word);
}


INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, INT nCmdShow) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // magic
    MO::AwareDPI();

    #ifdef _DEBUG
    MO::RedirectOutput();
    #endif

    if(MO::ShowMainWindow(hInstance, nCmdShow) == nullptr)
        return 0;

    auto hook = MO::CreateKbllHook();
    if(hook == nullptr)
        return 0;

    auto capturer = MO::ScreenCapturer();
    if(!capturer)
        return 0;

    auto ocre = MO::OcrWrapper();
    if(!ocre)
        return 0;

    MSG msg{};
    while(GetMessage(&msg, nullptr, 0, 0) > 0) {
        if(msg.message == WM_USER) {
            HandleKey(capturer, ocre, msg.wParam);
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    hook.reset();

    #ifdef _DEBUG
    MO::RestoreOutput();
    #endif

    return 0;
}
