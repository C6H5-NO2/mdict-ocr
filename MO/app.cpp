#include "app.h"

#include <winrt/Windows.Graphics.Imaging.h>

#include <Windows.h>

#include <locale>


namespace MO {
    std::wstring GetWord(const ScreenCapturer& capturer,
                         const OcrWrapper& ocre,
                         const int width,
                         const int height,
                         const float distanceBias) {
        auto bytes = capturer.Capture(std::make_pair(width, height));
        if(bytes == nullptr)
            return L"";

        auto sbm = BytesToSbm(bytes.get(), width, height);
        auto word = ocre.Recognize(sbm, distanceBias);
        if(word.length() == 0)
            return L"";

        // todo: set by LangTag
        const auto isPunct = [](auto chr) { return std::ispunct(chr, std::locale("en_US.UTF-8")); };
        word.erase(word.cbegin(), std::find_if_not(word.cbegin(), word.cend(), isPunct));
        word.erase(std::find_if_not(word.crbegin(), word.crend(), isPunct).base(), word.cend());
        return word;
    }


    bool SendWord(std::wstring word, const bool sendKeyup) {
        const auto keyCount = (2 + word.length()) * (sendKeyup ? 2 : 1);
        const auto keys = std::make_unique<INPUT[]>(keyCount);
        memset(keys.get(), 0, keyCount * sizeof(INPUT));

        auto currIdx = 0;
        const auto addKeyup = [&]() {
            keys[currIdx] = keys[currIdx - 1];
            keys[currIdx].ki.dwFlags |= KEYEVENTF_KEYUP;
            ++currIdx;
        };

        keys[currIdx].type = INPUT_KEYBOARD;
        keys[currIdx].ki.wVk = VK_RETURN; // first enter to gain focus of search bar
        ++currIdx;
        if(sendKeyup)
            addKeyup();

        for(const auto& chr : word) {
            keys[currIdx].type = INPUT_KEYBOARD;
            keys[currIdx].ki.wVk = 0;
            keys[currIdx].ki.wScan = chr;
            keys[currIdx].ki.dwFlags = KEYEVENTF_UNICODE;
            ++currIdx;
            if(sendKeyup)
                addKeyup();
        }

        keys[currIdx].type = INPUT_KEYBOARD;
        keys[currIdx].ki.wVk = VK_RETURN; // second enter to do search
        ++currIdx;
        if(sendKeyup)
            addKeyup();

        SendInput(keyCount, keys.get(), sizeof(INPUT));

        return true;
    }
}
