#pragma once
#ifndef MO_IMAGE_H_
#define MO_IMAGE_H_

#include <Windows.h>

#include <memory>


namespace winrt::Windows::Graphics::Imaging {
    struct SoftwareBitmap;
}


namespace MO {
    using Bytes = std::unique_ptr<unsigned char[]>;

    Bytes BitmapToBytes(HBITMAP bm, HDC dc);

    winrt::Windows::Graphics::Imaging::SoftwareBitmap BytesToSbm(const unsigned char* bytes, const int width, const int height);
}


#endif
