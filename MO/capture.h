#pragma once
#ifndef MO_CAPTURE_H_
#define MO_CAPTURE_H_

#include "image.h"


namespace MO {
    class ScreenCapturer {
    public:
        explicit ScreenCapturer();

        ScreenCapturer(const ScreenCapturer&) = delete;

        ScreenCapturer(ScreenCapturer&& other) noexcept;

        ~ScreenCapturer();

        explicit operator bool() const noexcept;

        Bytes Capture(const int width, const int height) const;


    protected:
        HDC ScrDC, MemDC;
    };
}


#endif
