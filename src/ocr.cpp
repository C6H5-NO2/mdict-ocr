#include "ocr.h"

#include <winrt/Windows.Globalization.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Media.Ocr.h>


using namespace winrt::Windows::Globalization;
using namespace winrt::Windows::Graphics::Imaging;
using namespace winrt::Windows::Media::Ocr;


std::wstring ocr(const SoftwareBitmap& sbm, const int distance_threshold) {
    assert((std::max(sbm.PixelWidth(), sbm.PixelHeight()) <= OcrEngine::MaxImageDimension()));

    const auto lang = Language(L"en-US");
    auto engine = OcrEngine::TryCreateFromLanguage(lang);
    if(!engine) {
        fprintf(stderr, "lang [%ls] is not supported.", lang.LanguageTag().c_str());
        return L"";
    }

    auto result = engine.RecognizeAsync(sbm).get();
    // todo: handle rotation
    //auto angle = result.TextAngle().Value();

    auto min_dis = sbm.PixelWidth() + sbm.PixelHeight();
    auto opt_word = winrt::hstring();

    for(const auto& line : result.Lines()) {
        for(const auto& word : line.Words()) {
            const auto aabb = word.BoundingRect();
            const auto center_x = aabb.X + aabb.Width / 2;
            const auto center_y = aabb.Y + aabb.Height / 2;

            const auto dis = std::abs(center_x - sbm.PixelWidth() / 2) + std::abs(center_y - sbm.PixelHeight() / 2);

            // todo: use mouse position to test instead
            if(dis < distance_threshold && dis < min_dis) {
                min_dis = dis;
                opt_word = word.Text();
            }
        }
    }

    return opt_word.c_str();
}
