/* main.cpp */

#include <iostream> // для отладки

#include "symbol_slicer/slicer.h"

int main(int /*argc*/, char **argv) {
    Magick::InitializeMagick(argv[0]);


    Magick::Image img("../images/f_test.png");
    SymbolSlicer::Slicer s(img);
    s.slice();

    //img.type(Magick::GrayscaleType);
    //img.write("../images/ans.jpg");
}
