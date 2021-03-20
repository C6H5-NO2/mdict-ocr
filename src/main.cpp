#include <cstdio>

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Globalization.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Media.Ocr.h>
#include <winrt/Windows.Storage.h>

using namespace winrt::Windows;


int main() {
    const auto test_imgpath = L"path\\to\\test.png";
    Storage::StorageFile file = nullptr;
    try {
        auto co = Storage::StorageFile::GetFileFromPathAsync(test_imgpath);
        file = co.get();
    }
    catch(const winrt::hresult_error& e) {
        fprintf(stderr, "code = %d\n", e.code().value);
        fprintf(stderr, "msg = %ls\n", e.message().c_str());
        return 1;
    }

    auto stream = file.OpenAsync(Storage::FileAccessMode::Read).get();
    auto decoder = Graphics::Imaging::BitmapDecoder::CreateAsync(stream).get();
    auto bitmap = decoder.GetSoftwareBitmapAsync().get();

    const auto lang = Globalization::Language(L"en-US");
    auto engine = Media::Ocr::OcrEngine::TryCreateFromLanguage(lang);
    if(!engine) {
        fprintf(stderr, "lang [%ls] is not supported.", lang.LanguageTag().c_str());
        return 1;
    }

    auto result = engine.RecognizeAsync(bitmap).get();

    //printf("%ls", result.Text().c_str());

    for(auto&& line : result.Lines()) {
        static int lno = 0;
        printf("line: %d\n", lno);
        lno++;
        for(auto&& word : line.Words()) {
            printf("%ls ", word.Text().c_str());
        }
        puts("\n");
    }

    return 0;
}
