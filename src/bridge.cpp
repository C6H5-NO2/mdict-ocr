#include "bridge.h"

#include <MemoryBuffer.h>
#include <Unknwn.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Graphics.Imaging.h>

#include <cstring>


using namespace winrt::Windows::Graphics::Imaging;


SoftwareBitmap bytes_to_sbm(const unsigned char* bytes, const int width, const int height) {
    auto sbm = SoftwareBitmap(BitmapPixelFormat::Bgra8, width, height, BitmapAlphaMode::Premultiplied);
    auto buf = sbm.LockBuffer(BitmapBufferAccessMode::Write);
    auto ref = buf.CreateReference();

    unsigned char* sbm_value;
    unsigned capacity;
    ref.as<Windows::Foundation::IMemoryBufferByteAccess>()->GetBuffer(&sbm_value, &capacity);

    assert((buf.GetPlaneDescription(0).StartIndex == 0));
    assert((buf.GetPlaneDescription(0).Stride == width * 4));

    memcpy(sbm_value, bytes, width * height * 4);

    return sbm;
}
