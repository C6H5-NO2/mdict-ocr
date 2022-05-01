#pragma once
#ifndef MO_UTIL_H_
#define MO_UTIL_H_

#include <memory>
#include <string>


namespace MO {
    constexpr auto CLASS_NAME = L"MDict OCR Wrapper Class";
    constexpr auto WINDOW_NAME = L"MDict OCR Wrapper";
    constexpr auto MDICT_WINDOW_NAME = L"MDict";
    constexpr auto WM_PICK_WORD = 0x0400u + 11;


    template<typename T>
    using UniqueHandle = std::enable_if_t<std::is_pointer_v<T>, std::unique_ptr<std::remove_pointer_t<T>, void(*)(T)>>;


    /// @note Call this function only once.
    void AwareDPI();


    bool SendWord(const std::wstring& word, const bool sendKeyup = false);
}


#endif
