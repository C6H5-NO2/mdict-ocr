#pragma once
#ifndef MO_UTIL_H_
#define MO_UTIL_H_

#include <memory>
#include <string>


//typedef struct HBITMAP__* HBITMAP;
//
//typedef struct HDC__* HDC;

#include <Windows.h>


namespace winrt::Windows::Graphics::Imaging {
    struct SoftwareBitmap;
}


namespace MO {
    const auto CLASS_NAME = L"MDict OCR Wrapper Class";
    const auto WINDOW_NAME = L"MDict OCR Wrapper";
    const auto MDICT = L"MDict";


    using Bytes = std::unique_ptr<unsigned char[]>;


    Bytes BitmapToBytes(HBITMAP bm, HDC dc);


    winrt::Windows::Graphics::Imaging::SoftwareBitmap BytesToSbm(const unsigned char* bytes, const int width, const int height);


    template<typename T>
    using UniqueHandle = std::enable_if_t<
        std::is_pointer_v<T>,
        std::unique_ptr<std::remove_pointer_t<T>, void(*)(T)>
    >;
}


#endif
