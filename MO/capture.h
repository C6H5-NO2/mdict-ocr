#pragma once

#include "util.h"

#include <utility>


namespace MO {
    /// @note Call this function only once.
    void AwareDPI();


    class ScreenCapturer {
    public:
        explicit ScreenCapturer();

        ScreenCapturer(const ScreenCapturer&) = delete;

        ScreenCapturer(ScreenCapturer&& other) noexcept;

        ~ScreenCapturer();

        explicit operator bool() const noexcept;

        /// @param dims First param is width, second be height.
        Bytes Capture(const std::pair<int, int> dims) const;

    protected:
        HDC ScrDC, MemDC;
    };
}
