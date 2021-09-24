#pragma once
#ifndef MO_WINDOW_H_
#define MO_WINDOW_H_

#include "util.h"

#include <Windows.h>


namespace MO {
    HWND ShowMainWindow(HINSTANCE hInstance, INT nCmdShow);

    UniqueHandle<HHOOK> CreateKbllHook();

    bool MoveToFront(HWND hwnd);
}


#endif
