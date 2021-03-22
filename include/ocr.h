#pragma once

#include <string>


namespace winrt::Windows::Graphics::Imaging {
    struct SoftwareBitmap;
}


// todo: make as class
/// @param distance_threshold maximum tolerate manhattan distance
std::wstring ocr(const winrt::Windows::Graphics::Imaging::SoftwareBitmap& sbm,
                 const int distance_threshold);
