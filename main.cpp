#include "MO/app.h"
#include "MO/capture.h"
#include "MO/console.h"
#include "MO/ocr.h"
#include "MO/ui.h"
#include "MO/util.h"
#include "MO/window.h"

#include <Windows.h>

#include <chrono>
#include <thread>


INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, INT nCmdShow) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // magic
    MO::AwareDPI();

    #ifdef _DEBUG
    MO::RedirectOutput();
    #endif

    auto hwnd = MO::ShowMainWindow(hInstance, nCmdShow);
    if(hwnd == nullptr)
        return 0;

    auto kbhook = MO::CreateKbllHook();
    if(kbhook == nullptr)
        return 0;

    MO::ScreenCapturer capturer;
    if(!capturer)
        return 0;

    MO::OcrWrapper ocre;
    if(!ocre)
        return 0;

    MO::UIWrapper ui(hwnd);

    MO::App app{ &capturer, &ocre, &ui };

    ui.Connect(&app);

    MSG msg{};
    while(GetMessage(&msg, nullptr, 0, 0) > 0) {
        if(ui.FilterMessage(&msg))
            continue;
        if(msg.message == MO::WM_PICK_WORD)
            app.HandleKey(msg.wParam);
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    kbhook.reset();

    #ifdef _DEBUG
    MO::RestoreOutput();
    #endif

    return 0;
}
