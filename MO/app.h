#pragma once
#ifndef MO_APP_H_
#define MO_APP_H_

#include "capture.h"
#include "ocr.h"

#include <string>


namespace MO {
    std::wstring GetWord(const ScreenCapturer& capturer,
                         const OcrWrapper& ocre,
                         const int width = 600,
                         const int height = 200,
                         const float distanceBias = 20);

    bool SendWord(std::wstring word, const bool sendKeyup = false);
}


#endif
