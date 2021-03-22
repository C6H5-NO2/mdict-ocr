#include "capture.h"
#include "bridge.h"
#include "ocr.h"

#include <winrt/Windows.Graphics.Imaging.h>


int main() {
    const auto width = 600, height = 200;
    
    aware_dpi();

    auto bytes = capture(width, height);
    auto sbm = bytes_to_sbm(bytes.get(), width, height);
    auto word = ocr(sbm, 20);

    printf("%ls", word.c_str());

    return 0;
}
