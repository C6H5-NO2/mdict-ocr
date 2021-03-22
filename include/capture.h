#pragma once

#include <memory>


/// @brief Call this function once.
void aware_dpi();


std::unique_ptr<unsigned char[]> capture(const int width, const int height);
