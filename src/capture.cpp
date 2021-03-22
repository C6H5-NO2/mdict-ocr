#include "capture.h"

#include <Windows.h>
#include <wingdi.h>
#include <winuser.h>

#include <functional>

#include <cassert>


void aware_dpi() {
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
}


template<typename T>
using unique_handle = std::enable_if_t<
    std::is_pointer_v<T>,
    std::unique_ptr<std::remove_pointer_t<T>, std::function<void(T)>>
>;


auto hbitmap_to_bytes(const unique_handle<HBITMAP>& bm, const unique_handle<HDC>& dc) {
    BITMAP bm_obj;
    GetObject(bm.get(), sizeof(BITMAP), &bm_obj);
    assert((bm_obj.bmPlanes == 1));
    assert((bm_obj.bmBitsPixel == 32));

    BITMAPINFO bm_info;
    bm_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bm_info.bmiHeader.biWidth = bm_obj.bmWidth;
    bm_info.bmiHeader.biHeight = -bm_obj.bmHeight;
    bm_info.bmiHeader.biPlanes = 1;
    bm_info.bmiHeader.biBitCount = 32;
    bm_info.bmiHeader.biCompression = BI_RGB;
    bm_info.bmiHeader.biSizeImage = 0;
    bm_info.bmiHeader.biXPelsPerMeter = 0;
    bm_info.bmiHeader.biYPelsPerMeter = 0;
    bm_info.bmiHeader.biClrUsed = 0;
    bm_info.bmiHeader.biClrImportant = 0;

    const auto bm_size = bm_obj.bmWidth * bm_obj.bmHeight * 4; // 32 bits -> 4 bytes
    auto bytes = std::make_unique<unsigned char[]>(bm_size);
    GetDIBits(dc.get(), bm.get(), 0, bm_obj.bmHeight, bytes.get(), &bm_info, DIB_RGB_COLORS);
    return bytes;
}


std::unique_ptr<unsigned char[]> capture(const int width, const int height) {
    unique_handle<HDC> scr_dc(
        GetDC(nullptr),
        [](auto dc) { ReleaseDC(nullptr, dc); }
    );
    unique_handle<HDC> mem_dc(
        CreateCompatibleDC(scr_dc.get()),
        [](auto dc) { DeleteDC(dc); }
    );
    unique_handle<HBITMAP> mem_bm(
        CreateCompatibleBitmap(scr_dc.get(), width, height),
        [](auto bm) { DeleteObject(bm); }
    );
    unique_handle<HGDIOBJ> ori_bm(
        SelectObject(mem_dc.get(), mem_bm.get()),
        [dc = mem_dc.get()](auto bm) { SelectObject(dc, bm); }
    );

    POINT mouse_pos;
    GetCursorPos(&mouse_pos);

    // capture screen
    BitBlt(mem_dc.get(),
           0, 0,
           width, height,
           scr_dc.get(),
           mouse_pos.x - width / 2, mouse_pos.y - height / 2,
           SRCCOPY | BLACKNESS);

    auto buffer = hbitmap_to_bytes(mem_bm, scr_dc);

    ori_bm = nullptr;
    mem_bm = nullptr;
    mem_dc = nullptr;
    scr_dc = nullptr;

    return buffer;
}
