/* main.cpp */

#include <iostream> // для отладки

#include "symbol_slicer/slicer.h"

int main(int /*argc*/, char **argv) {
    Magick::InitializeMagick(argv[0]);


    SymbolSlicer::Slicer s;
    Magick::Image img("../images/test1.jpg");
    s.slice(img);

    //img.type(Magick::GrayscaleType);
    img.write("../images/ans.jpg");
}
