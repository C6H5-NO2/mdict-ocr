#include "util.h"

#include <Unknwn.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Graphics.Imaging.h>

#include <MemoryBuffer.h>

#include <Windows.h>


using namespace winrt::Windows::Graphics::Imaging;


namespace MO {
    Bytes BitmapToBytes(HBITMAP bm, HDC dc) {
        BITMAP bmObj;
        GetObject(bm, sizeof(BITMAP), &bmObj);
        assert((bmObj.bmPlanes == 1) && "bmPlanes == 1");
        assert((bmObj.bmBitsPixel == 32) && "bmBitsPixel == 32");

        BITMAPINFO bmInfo;
        bmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmInfo.bmiHeader.biWidth = bmObj.bmWidth;
        bmInfo.bmiHeader.biHeight = -bmObj.bmHeight;
        bmInfo.bmiHeader.biPlanes = 1;
        bmInfo.bmiHeader.biBitCount = 32;
        bmInfo.bmiHeader.biCompression = BI_RGB;
        bmInfo.bmiHeader.biSizeImage = 0;
        bmInfo.bmiHeader.biXPelsPerMeter = 0;
        bmInfo.bmiHeader.biYPelsPerMeter = 0;
        bmInfo.bmiHeader.biClrUsed = 0;
        bmInfo.bmiHeader.biClrImportant = 0;

        const auto bmSize = bmObj.bmWidth * bmObj.bmHeight * 4; // 32 bits => 4 bytes
        auto bytes = std::make_unique<unsigned char[]>(bmSize); // make_shared since C++20
        GetDIBits(dc, bm, 0, bmObj.bmHeight, bytes.get(), &bmInfo, DIB_RGB_COLORS);
        return bytes;
    }


    SoftwareBitmap BytesToSbm(const unsigned char* bytes, const int width, const int height) {
        auto sbm = SoftwareBitmap(BitmapPixelFormat::Bgra8, width, height, BitmapAlphaMode::Premultiplied);
        auto buf = sbm.LockBuffer(BitmapBufferAccessMode::Write);
        auto ref = buf.CreateReference();

        unsigned char* sbmValue;
        unsigned capacity;
        ref.as<Windows::Foundation::IMemoryBufferByteAccess>()->GetBuffer(&sbmValue, &capacity);

        assert((buf.GetPlaneDescription(0).StartIndex == 0) && "StartIndex == 0");
        assert((buf.GetPlaneDescription(0).Stride == width * 4) && "Stride == width * 4");

        memcpy(sbmValue, bytes, width * height * 4);

        return sbm;
    }
}
