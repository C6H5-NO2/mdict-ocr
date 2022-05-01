#pragma once
#ifndef MO_OCR_H_
#define MO_OCR_H_

#include <winrt/Windows.Media.Ocr.h>

#include <string>
#include <vector>


namespace MO {
    class OcrWrapper {
    public:
        explicit OcrWrapper(const std::wstring& langTag = L"en-US");

        explicit operator bool() const noexcept;

        /// @deprecated Dirty API
        void HotSwitchLanguage(const std::wstring& langTag);

        std::wstring Recognize(const winrt::Windows::Graphics::Imaging::SoftwareBitmap& sbm, const float distanceBias) const;

        std::wstring LanguageTag() const noexcept;

        static std::vector<std::wstring> AvailableLanguages() noexcept;


    protected:
        winrt::Windows::Media::Ocr::OcrEngine Engine;
    };
}


#endif
