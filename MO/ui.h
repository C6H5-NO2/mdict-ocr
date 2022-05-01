#pragma once
#ifndef MO_UI_H_
#define MO_UI_H_

#include <Unknwn.h>

#include <winrt/Windows.UI.Xaml.Hosting.h>

#include <Windows.h>


namespace MO {
    struct App;

    struct UIImpl;


    class UIWrapper {
    public:
        explicit UIWrapper(HWND hwnd);

        bool FilterMessage(const MSG* message) const;

        void Connect(const App* app);

        std::wstring GetSearchText() const;

        void SetSearchText(const std::wstring& text) const;

        bool IsAutoForward() const;


    protected:
        winrt::Windows::UI::Xaml::Hosting::DesktopWindowXamlSource Source;
        std::shared_ptr<UIImpl> Impl;
    };
}


#endif
