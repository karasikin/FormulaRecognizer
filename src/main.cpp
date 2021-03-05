/* main.cpp */

#include <iostream> // для отладки

#include "segmenter/formula.h"

int main(int /*argc*/, char **argv) {
    Magick::InitializeMagick(argv[0]);


    Magick::Image img("../images/f_test.png");
    Segmenter::Formula formula(img);
    formula.slice();

    //img.type(Magick::GrayscaleType);
    //img.write("../images/ans.jpg");
}
