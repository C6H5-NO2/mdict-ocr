#pragma once
#ifndef MO_OCR_H_
#define MO_OCR_H_

#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Media.Ocr.h>

#include <string>


namespace MO {
    class OcrWrapper {
    public:
        explicit OcrWrapper(std::wstring langTag = L"en-US");

        OcrWrapper(const OcrWrapper&) = delete;

        OcrWrapper(OcrWrapper&& other) noexcept;

        explicit operator bool() const noexcept;

        std::wstring Recognize(const winrt::Windows::Graphics::Imaging::SoftwareBitmap& sbm, const float distanceBias) const;

        std::wstring LanguageTag() const noexcept;

    protected:
        winrt::Windows::Media::Ocr::OcrEngine Engine;
    };
}


#endif
