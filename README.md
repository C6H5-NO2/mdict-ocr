# mdict-ocr
`mdict-ocr` provides an optical character recognition (OCR) wrapper for the [MDict app](https://www.mdict.cn/).

![](https://user-images.githubusercontent.com/19368807/166148204-5095d23a-9e4f-48e3-976b-116d9add75a1.png)



## Prerequisites
- Windows 10 with corresponding language packs installed
- MDict for PC 2.0.12


## Usage
0. Install language packs through the Settings app. (See [Windows support](https://support.microsoft.com/en-us/windows/language-packs-for-windows-a5094319-a92d-18de-5b53-1cfc697cfca8))  
   The target language is not necessarily the display language but the OCR feature should be installed.  

1. Start the MDict app. Make sure that it is the only window named `mdict` (case-insensitive).

2. Hover the cursor on the word to be recognized and hit left control.  
   This will bring the MDict window to top and perform a query automatically.  


## Developing
0. The project uses [WinUI 2](https://docs.microsoft.com/en-us/windows/apps/winui/winui2) and [XAML Islands](https://docs.microsoft.com/en-us/windows/apps/desktop/modernize/xaml-islands), requiring the following NuGet packages to be installed.
    - [Microsoft.UI.Xaml](https://www.nuget.org/packages/Microsoft.UI.Xaml)
    - [Microsoft.Windows.CppWinRT](https://www.nuget.org/packages/Microsoft.Windows.CppWinRT)
    - [Microsoft.Toolkit.Win32.UI.SDK](https://www.nuget.org/packages/Microsoft.Toolkit.Win32.UI.SDK)

1. Add a reference to the Windows Runtime metadata, which is under `C:/Program Files (x86)/Windows Kits/10/UnionMetadata/[Windows 10 SDK version]/Windows.winmd`. (See [step 6 here](https://docs.microsoft.com/en-us/windows/apps/desktop/modernize/host-standard-control-with-xaml-islands-cpp#create-a-desktop-application-project))

2. Some relevant property settings for the compiler.  
```
/D "_UNICODE" /D "UNICODE" /MD /std:c++17
```
  
3. And those for the linker.
```
/DYNAMICBASE "C:/Program Files (x86)/Windows Kits/10/Lib/[Windows 10 SDK version]/um/x64/WindowsApp.lib" /SUBSYSTEM:WINDOWS
```


## Known Issues
Cannot move MDict to front when a system app (e.g. Task Manager) gains the foreground focus.


## License
Distributed under the MIT License.
