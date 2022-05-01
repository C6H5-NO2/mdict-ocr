#include "util.h"

#include <Windows.h>


namespace MO {
    void AwareDPI() {
        SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
    }


    bool SendWord(const std::wstring& word, const bool sendKeyup) {
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
