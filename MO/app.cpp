#include "app.h"

#include "capture.h"
#include "image.h"
#include "ocr.h"
#include "ui.h"
#include "util.h"
#include "window.h"

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.UI.Xaml.Controls.h>

#include <cwctype>


using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Xaml;


namespace MO {
    void App::HandleKey(const unsigned vkCode) const {
        fprintf_s(stdout, "[DEBUG] vkCode: %lu\n", vkCode);
        // use left control as hot key
        if(vkCode != VK_LCONTROL)
            return;

        auto word = DoRecognize();
        if(word.length() == 0)
            return;

        fprintf_s(stdout, "[DEBUG] OCR: '%ls", word.c_str());
        fprintf_s(stdout, "'\n");
        SetSearchText(word);

        if(word.length() > MaxSearchLength) // if too long
            return;

        if(!IsAutoForward())
            return;

        SendSearchText(word);
    }


    std::wstring App::DoRecognize(const int width, const int height, const float distanceBias) const {
        auto bytes = Capturer->Capture(width, height);
        if(bytes == nullptr)
            return L"";

        auto sbm = BytesToSbm(bytes.get(), width, height);
        auto word = Ocre->Recognize(sbm, distanceBias);
        if(word.length() == 0)
            return L"";

        word.erase(word.cbegin(), std::find_if_not(word.cbegin(), word.cend(), std::iswpunct));
        word.erase(std::find_if_not(word.crbegin(), word.crend(), std::iswpunct).base(), word.cend());
        return word;
    }


    bool App::SendSearchText(const std::wstring& text) const {
        auto mdictHwnd = FindWindow(nullptr, MDICT_WINDOW_NAME);
        if(mdictHwnd == nullptr) {
            fprintf_s(stderr, "[WARN] MDict window not found.\n");
            return false;
        }

        if(MoveToFront(mdictHwnd) == false) {
            fprintf_s(stderr, "[ERROR] Failed to move MDict window to front.\n");
            return false;
        }

        return SendWord(text);
    }


    std::wstring App::GetSearchText() const {
        return UI->GetSearchText();
    }


    void App::SetSearchText(const std::wstring& text) const {
        UI->SetSearchText(text);
    }


    bool App::IsAutoForward() const {
        return UI->IsAutoForward();
    }


    void App::OnLanguageSelectionChanged(const IInspectable& sender, const Controls::SelectionChangedEventArgs& args) const {
        auto lang = winrt::unbox_value<winrt::hstring>(args.AddedItems().GetAt(0));
        if(std::wcscmp(lang.c_str(), Ocre->LanguageTag().c_str()) != 0) {
            fprintf_s(stdout, "[INFO] OCR language switch to '%ls", lang.c_str());
            fprintf_s(stdout, "'\n");
            Ocre->HotSwitchLanguage(lang.c_str());
        }
    }


    void App::OnForwardButtonClicked(const IInspectable& sender, const RoutedEventArgs& args) const {
        auto word = GetSearchText();
        if(word.length() == 0 || word.length() > MaxSearchLength)
            return;
        SendSearchText(word);
    }
}
