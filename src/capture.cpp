#include <Windows.h>
#include <wingdi.h>
#include <WinUser.h>

void capture() {
    SetProcessDPIAware();

    const auto w = 600, h = 200;

    auto screen_dc = GetDC(nullptr);
    auto memory_dc = CreateCompatibleDC(screen_dc);
    auto memory_bm = CreateCompatibleBitmap(screen_dc, w, h);
    auto origin_bm = SelectObject(memory_dc, memory_bm);

    POINT mouse_pos;
    GetCursorPos(&mouse_pos);

    BitBlt(memory_dc, 0, 0, w, h, screen_dc, mouse_pos.x - w / 2, mouse_pos.y - h / 2, SRCCOPY | BLACKNESS);

    OpenClipboard(nullptr);
    EmptyClipboard();
    SetClipboardData(CF_BITMAP, memory_bm);
    CloseClipboard();

    SelectObject(memory_dc, origin_bm);
    DeleteObject(memory_bm);
    DeleteDC(memory_dc);
    ReleaseDC(nullptr, screen_dc);
}
