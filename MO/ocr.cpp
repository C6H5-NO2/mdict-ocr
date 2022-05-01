#include "ocr.h"

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Globalization.h>
#include <winrt/Windows.Graphics.Imaging.h>

#include <future>


using namespace winrt::Windows::Globalization;
using namespace winrt::Windows::Graphics::Imaging;
using namespace winrt::Windows::Media::Ocr;


namespace MO {
    OcrWrapper::OcrWrapper(const std::wstring& langTag) : Engine(OcrEngine::TryCreateFromLanguage(Language(langTag))) {
        if(Engine == nullptr) {
            fprintf_s(stderr, "[ERROR] Language '%ls", langTag.c_str());
            fprintf_s(stderr, "' is not supported.\n");
        }
    }


    OcrWrapper::operator bool() const noexcept {
        return !!Engine;
    }


    void OcrWrapper::HotSwitchLanguage(const std::wstring& langTag) {
        Engine = OcrEngine::TryCreateFromLanguage(Language(langTag));
    }


    std::wstring OcrWrapper::Recognize(const SoftwareBitmap& sbm, const float distanceBias) const {
        if(std::max(sbm.PixelWidth(), sbm.PixelHeight()) > OcrEngine::MaxImageDimension()) {
            fprintf_s(stderr, "[ERROR] Max image dimension %u exceeded.\n", OcrEngine::MaxImageDimension());
            return L"";
        }

        //auto result = co_await Engine.RecognizeAsync(sbm);
        //auto result = Engine.RecognizeAsync(sbm).get();

        // MSVC known issue: <T> in std::future is incorrectly required to be default constructable
        struct OcrResultWrapper {
            OcrResult Result = nullptr;
        };
        // let it block. default launch policy suffices
        auto fut = std::async([](const OcrEngine& engine, const SoftwareBitmap& sbm) {
            OcrResultWrapper wrapper;
            wrapper.Result = engine.RecognizeAsync(sbm).get();
            return wrapper;
        }, Engine, sbm);
        // get immediately to prevent dangling
        auto result = fut.get().Result;

        // todo: handle rotation
        //auto angle = result.TextAngle().Value();

        const auto px = sbm.PixelWidth() / 2, py = sbm.PixelHeight() / 2;
        auto minDis = distanceBias + 1;
        auto optWord = winrt::hstring();

        for(const auto& line : result.Lines()) {
            for(const auto& word : line.Words()) {
                const auto aabb = word.BoundingRect();
                const auto cx = aabb.X + aabb.Width / 2, cy = aabb.Y + aabb.Height / 2;
                const auto dx = std::max(std::abs(px - cx) - aabb.Width / 2, .0f);
                const auto dy = std::max(std::abs(py - cy) - aabb.Height / 2, .0f);
                const auto dis = std::sqrt(dx * dx + dy * dy);

                // exact hit
                if(dis < 1e-5f)
                    return word.Text().c_str();

                // within bias
                if(dis < distanceBias && dis < minDis) {
                    minDis = dis;
                    optWord = word.Text();
                }
            }
        }

        return optWord.c_str();
    }


    std::wstring OcrWrapper::LanguageTag() const noexcept {
        return Engine.RecognizerLanguage().LanguageTag().c_str();
    }


    std::vector<std::wstring> OcrWrapper::AvailableLanguages() noexcept {
        auto langs = OcrEngine::AvailableRecognizerLanguages();
        std::vector<std::wstring> ret;
        for(auto idx = 0u; idx < langs.Size(); ++idx)
            ret.emplace_back(langs.GetAt(idx).LanguageTag().c_str());
        return ret;
    }
}
