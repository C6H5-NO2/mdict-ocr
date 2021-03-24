#pragma once

#include <memory>
// todo: consider using the new IGraphicsCaptureItemInterop::CreateForWindow() api

/// @brief Call this function once.
void aware_dpi();


std::unique_ptr<unsigned char[]> capture(const int width, const int height);
