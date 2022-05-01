#include "capture.h"

#include <Windows.h>


namespace MO {
    // todo: consider using the new IGraphicsCaptureItemInterop::CreateForWindow() api
    ScreenCapturer::ScreenCapturer() : ScrDC(GetDC(nullptr)),
                                       MemDC(CreateCompatibleDC(ScrDC)) {
        if(ScrDC == nullptr || MemDC == nullptr)
            fprintf_s(stderr, "[ERROR] Failed to retrieve device context.\n");
    }


    ScreenCapturer::ScreenCapturer(ScreenCapturer&& other) noexcept : ScrDC(other.ScrDC),
                                                                      MemDC(other.MemDC) {
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


    Bytes ScreenCapturer::Capture(const int width, const int height) const {
        auto memBM = CreateCompatibleBitmap(ScrDC, width, height);
        auto prvBM = SelectObject(MemDC, memBM);

        POINT cursorPos;
        GetCursorPos(&cursorPos);

        Bytes buffer = nullptr;

        // do capture
        if(FALSE == BitBlt(MemDC, 0, 0, width, height,
                           ScrDC, cursorPos.x - width / 2, cursorPos.y - height / 2, SRCCOPY | BLACKNESS)) {
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
