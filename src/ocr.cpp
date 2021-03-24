#include "ocr.h"

#include <winrt/Windows.Globalization.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Media.Ocr.h>


using namespace winrt::Windows::Globalization;
using namespace winrt::Windows::Graphics::Imaging;
using namespace winrt::Windows::Media::Ocr;


std::wstring ocr(const SoftwareBitmap& sbm, const float distance_threshold) {
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

    const auto px = sbm.PixelWidth() / 2, py = sbm.PixelHeight() / 2;
    auto min_dis = distance_threshold + 1;
    auto opt_word = winrt::hstring();

    for(const auto& line : result.Lines()) {
        for(const auto& word : line.Words()) {
            const auto aabb = word.BoundingRect();
            const auto cx = aabb.X + aabb.Width / 2, cy = aabb.Y + aabb.Height / 2;
            const auto dx = std::max(std::abs(px - cx) - aabb.Width / 2, .0f);
            const auto dy = std::max(std::abs(py - cy) - aabb.Height / 2, .0f);
            const auto dis = std::sqrt(dx * dx + dy * dy);

            if(dis < 1e-5)
                return word.Text().c_str();

            if(dis < distance_threshold && dis < min_dis) {
                min_dis = dis;
                opt_word = word.Text();
            }
        }
    }

    return opt_word.c_str();
}
