#include "ui.h"

#include "app.h"
#include "ocr.h"

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.UI.Xaml.Controls.h>
#include <winrt/Windows.UI.Xaml.Controls.Primitives.h>

#include <windows.ui.xaml.hosting.desktopwindowxamlsource.h>


using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Xaml;


namespace MO {
    Hosting::DesktopWindowXamlSource InitXamlIsland(HWND hwnd) {
        winrt::init_apartment(winrt::apartment_type::single_threaded);
        //Hosting::WindowsXamlManager::InitializeForCurrentThread();
        Hosting::DesktopWindowXamlSource source;
        auto interop = source.as<IDesktopWindowXamlSourceNative>();
        interop->AttachToWindow(hwnd);

        HWND island = nullptr;
        interop->get_WindowHandle(&island);
        RECT rect;
        GetWindowRect(hwnd, &rect);
        SetWindowPos(island, nullptr, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);

        return source;
    }


    struct UIImpl {
        winrt::weak_ref<Controls::CheckBox> AutoFwdChk = nullptr;
        winrt::weak_ref<Controls::TextBox> RstTxt = nullptr;
    };


    UIWrapper::UIWrapper(HWND hwnd) : Source(InitXamlIsland(hwnd)),
                                      Impl(std::make_shared<UIImpl>()) {}


    bool UIWrapper::FilterMessage(const MSG* message) const {
        BOOL processed = false;
        Source.as<IDesktopWindowXamlSourceNative2>()->PreTranslateMessage(message, &processed);
        return processed;
    }


    void UIWrapper::Connect(const App* app) {
        Controls::StackPanel mainContainer;
        mainContainer.Spacing(10);
        mainContainer.Padding({ 20, 20, 20, 20 });

        // language selection part

        Controls::StackPanel langSelContainer;
        langSelContainer.Spacing(20);
        langSelContainer.Orientation(Controls::Orientation::Horizontal);

        Controls::Border langSelHeaderBorder; // vertical centering hack
        langSelHeaderBorder.BorderThickness({ 0, 0, 0, 0 });
        Controls::TextBlock langSelHeader;
        langSelHeader.Text(L"OCR Language: ");
        langSelHeader.TextAlignment(TextAlignment::Center);
        langSelHeader.VerticalAlignment(VerticalAlignment::Center);
        langSelHeaderBorder.Child(langSelHeader);
        langSelContainer.Children().Append(langSelHeaderBorder);

        Controls::ComboBox langSelBox;
        langSelBox.MinWidth(100);
        langSelBox.PlaceholderText(L"Select Language");
        if(const auto langs = OcrWrapper::AvailableLanguages(); !langs.empty()) {
            for(const auto& lang : langs)
                langSelBox.Items().Append(winrt::box_value(lang));
            langSelBox.SelectionChanged({ app, &App::OnLanguageSelectionChanged }); // small project, raw pointer suffices
            langSelBox.Loaded({ [sb=winrt::make_weak(langSelBox)](const auto&, const auto&) { sb.get().SelectedIndex(0); } });
        }
        langSelContainer.Children().Append(langSelBox);

        mainContainer.Children().Append(langSelContainer);

        // ocr result part

        Controls::StackPanel ocrRstContainer;
        ocrRstContainer.Spacing(30);
        ocrRstContainer.Orientation(Controls::Orientation::Horizontal);

        Controls::Border ocrRstHeaderBorder;
        ocrRstHeaderBorder.BorderThickness({ 0, 0, 0, 0 });
        Controls::TextBlock ocrRstHeader;
        ocrRstHeader.Text(L"OCR Result: ");
        ocrRstHeader.VerticalAlignment(VerticalAlignment::Center);
        ocrRstHeaderBorder.Child(ocrRstHeader);
        ocrRstContainer.Children().Append(ocrRstHeaderBorder);

        Controls::CheckBox autoFwdChk;
        autoFwdChk.Content(winrt::box_value(L"Auto Forward"));
        autoFwdChk.IsChecked(true);
        ocrRstContainer.Children().Append(autoFwdChk);
        Impl->AutoFwdChk = winrt::make_weak(autoFwdChk);

        Controls::Button fwdBtn;
        fwdBtn.Content(winrt::box_value(L"Forward"));
        fwdBtn.ClickMode(Controls::ClickMode::Release);
        fwdBtn.Click({ app, &App::OnForwardButtonClicked });
        ocrRstContainer.Children().Append(fwdBtn);

        mainContainer.Children().Append(ocrRstContainer);

        Controls::TextBox rstTxt;
        rstTxt.AcceptsReturn(true);
        Impl->RstTxt = winrt::make_weak(rstTxt);

        Controls::ScrollViewer rstSrl;
        rstSrl.Content(rstTxt);
        rstSrl.MaxHeight(100);
        mainContainer.Children().Append(rstSrl);

        // ||

        mainContainer.UpdateLayout();
        Source.Content(mainContainer);
    }


    std::wstring UIWrapper::GetSearchText() const {
        const auto& ele = Impl->RstTxt.get();
        return ele ? ele.Text().c_str() : L"";
    }


    void UIWrapper::SetSearchText(const std::wstring& text) const {
        if(const auto& ele = Impl->RstTxt.get())
            ele.Text(text.c_str());
    }


    bool UIWrapper::IsAutoForward() const {
        const auto& ele = Impl->AutoFwdChk.get();
        return ele ? ele.IsChecked().Value() : true;
    }
}
