#pragma once
#ifndef MO_APP_H_
#define MO_APP_H_

#include <string>


namespace winrt::Windows {
    namespace UI {
        namespace Xaml {
            struct RoutedEventArgs;

            namespace Controls {
                struct SelectionChangedEventArgs;
            }
        }
    }

    namespace Foundation {
        struct IInspectable;
    }
}


namespace MO {
    class ScreenCapturer;
    class OcrWrapper;
    class UIWrapper;


    struct App {
        void HandleKey(const unsigned vkCode) const;

        std::wstring DoRecognize(const int width = 600, const int height = 200, const float distanceBias = 20) const;

        bool SendSearchText(const std::wstring& text) const;

        std::wstring GetSearchText() const;

        void SetSearchText(const std::wstring& text) const;

        bool IsAutoForward() const;

        void OnLanguageSelectionChanged(const winrt::Windows::Foundation::IInspectable& sender,
                                        const winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs& args) const;

        void OnForwardButtonClicked(const winrt::Windows::Foundation::IInspectable& sender,
                                    const winrt::Windows::UI::Xaml::RoutedEventArgs& args) const;


        ScreenCapturer* Capturer = nullptr;
        OcrWrapper* Ocre = nullptr;
        UIWrapper* UI = nullptr;

        unsigned MaxSearchLength = 32;
    };
}


#endif
