#pragma once


namespace winrt::Windows::Graphics::Imaging {
    struct SoftwareBitmap;
}


winrt::Windows::Graphics::Imaging::SoftwareBitmap bytes_to_sbm(const unsigned char* bytes,
                                                               const int width,
                                                               const int height);
