# mdict-ocr
`mdict-ocr` provides an optical character recognition (OCR) wrapper for [MDict](https://www.mdict.cn/).


## Prerequisites
- Windows 10 with corresponding language packs installed
- MDict for PC 2.0.12


## Usage
0. Install language packs through the Settings app. (see [Windows support](https://support.microsoft.com/en-us/windows/language-packs-for-windows-a5094319-a92d-18de-5b53-1cfc697cfca8))  
   The target language is not necessarily the display language but the OCR feature should be installed.  

1. Start the MDict app. Make sure that it is the only one window named "MDict".

2. Put the cursor on the word to be recognised and hit left control.  
   This will bring the MDict window to top and perform a query automatically.  


## Developing
Essential property settings as follows.  

### Compiler
```
/D "_UNICODE" /D "UNICODE" /MD /std:c++17
```

### Linker
```
/DYNAMICBASE "C:/Program Files (x86)/Windows Kits/10/Lib/[Windows 10 SDK version]/um/x64/WindowsApp.lib" /SUBSYSTEM:WINDOWS
```


## License
Distributed under the MIT License.
