#include "capture.h"

#include <Windows.h>


namespace MO {
    void AwareDPI() {
        SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
    }


    // todo: consider using the new IGraphicsCaptureItemInterop::CreateForWindow() api
    ScreenCapturer::ScreenCapturer() : ScrDC(GetDC(nullptr)),
                                       MemDC(CreateCompatibleDC(ScrDC)) {
        if(ScrDC == nullptr || MemDC == nullptr)
            fprintf_s(stderr, "[ERROR] Failed to retrieve device context.\n");
    }


    ScreenCapturer::ScreenCapturer(ScreenCapturer&& other) noexcept {
        ScrDC = other.ScrDC;
        MemDC = other.MemDC;
        other.MemDC = nullptr;
        other.ScrDC = nullptr;
    }


    ScreenCapturer::~ScreenCapturer() {
        if(MemDC)
            DeleteDC(MemDC);
        if(ScrDC)
            ReleaseDC(nullptr, ScrDC);
    }


    ScreenCapturer::operator bool() const noexcept {
        return ScrDC && MemDC;
    }


    Bytes ScreenCapturer::Capture(const std::pair<int, int> dims) const {
        auto memBM = CreateCompatibleBitmap(ScrDC, dims.first, dims.second);
        auto prvBM = SelectObject(MemDC, memBM);

        POINT cursorPos;
        GetCursorPos(&cursorPos);

        Bytes buffer = nullptr;

        // do capture
        if(FALSE == BitBlt(MemDC, 0, 0, dims.first, dims.second, ScrDC,
                           cursorPos.x - dims.first / 2, cursorPos.y - dims.second / 2, SRCCOPY | BLACKNESS)) {
            fprintf_s(stderr, "[ERROR] Failed to capture screen. BitBlt() errno %lu\n", GetLastError());
        }
        else {
            buffer = BitmapToBytes(memBM, ScrDC);
        }

        SelectObject(MemDC, prvBM);
        DeleteObject(memBM);

        return buffer;
    }
}
