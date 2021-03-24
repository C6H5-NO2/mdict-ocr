#include "capture.h"
#include "bridge.h"
#include "ocr.h"

#include <Windows.h>

#include <winrt/Windows.Graphics.Imaging.h>

#include <chrono>
#include <thread>
#include <locale>


std::wstring get_word(const int width = 600, const int height = 200) {
    auto bytes = capture(width, height);
    auto sbm = bytes_to_sbm(bytes.get(), width, height);
    auto word = ocr(sbm, 20);
    const auto punct = [](auto chr) { return std::ispunct(chr, std::locale("en_US.UTF-8")); };
    if(word.length() == 0)
        return word;
    word.erase(word.cbegin(), std::find_if_not(word.cbegin(), word.cend(), punct));
    word.erase(std::find_if_not(word.crbegin(), word.crend(), punct).base(), word.cend());
    return word;
}


void send_word(const std::wstring& word, const bool include_keyup = false) {
    const auto key_count = (2 + word.length()) * (include_keyup ? 2 : 1);
    const auto keys = std::make_unique<INPUT[]>(key_count);
    memset(keys.get(), 0, key_count * sizeof(INPUT));

    auto curr_idx = 0;
    const auto add_keyup = [&]() {
        keys[curr_idx] = keys[curr_idx - 1];
        keys[curr_idx].ki.dwFlags |= KEYEVENTF_KEYUP;
        ++curr_idx;
    };

    keys[curr_idx].type = INPUT_KEYBOARD;
    keys[curr_idx].ki.wVk = VK_RETURN;
    ++curr_idx;
    if(include_keyup)
        add_keyup();

    for(const auto& chr : word) {
        keys[curr_idx].type = INPUT_KEYBOARD;
        keys[curr_idx].ki.wVk = 0;
        keys[curr_idx].ki.wScan = chr;
        keys[curr_idx].ki.dwFlags = KEYEVENTF_UNICODE;
        ++curr_idx;
        if(include_keyup)
            add_keyup();
    }

    keys[curr_idx].type = INPUT_KEYBOARD;
    keys[curr_idx].ki.wVk = VK_RETURN;
    ++curr_idx;
    if(include_keyup)
        add_keyup();

    SendInput(key_count, keys.get(), sizeof(INPUT));
}


int main() {
    //static bool quit_app = false;

    //SetConsoleCtrlHandler([](DWORD ctrl_type)-> BOOL {
    //    if(ctrl_type == CTRL_C_EVENT) {
    //        quit_app = true;
    //        return true;
    //    }
    //    return false;
    //}, true);

    try {
        const auto mdict_window = FindWindow(nullptr, L"MDict");
        if(mdict_window == nullptr)
            throw std::runtime_error("MDict window not found");

        // todo
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        aware_dpi();

        const auto word = get_word();
        if(word.length() == 0)
            return 0;
        printf("%ls\n", word.c_str());

        // bug: fail to set foreground when in background
        if(SetForegroundWindow(mdict_window) == 0) {
            const auto err = GetLastError();
            switch(err) {
            case ERROR_INVALID_WINDOW_HANDLE:
                throw std::runtime_error("MDict window closed");
            case ERROR_SUCCESS:
            case ERROR_ACCESS_DENIED:
                throw std::runtime_error("process is in background");
            default:
                throw std::runtime_error("SetForegroundWindow" + std::to_string(err));
            }
        }
        // force show window
        ShowWindow(mdict_window, SW_SHOWDEFAULT);

        send_word(word);
    }
    catch(const std::runtime_error& e) {
        fprintf(stderr, "%s\n", e.what());
    }

    return 0;
}
