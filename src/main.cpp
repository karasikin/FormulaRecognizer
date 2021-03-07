/* main.cpp */

#include <iostream> // для отладки

#include "segmenter/formula.h"

int main(int /*argc*/, char **argv) {
    Magick::InitializeMagick(argv[0]);


    auto img = std::make_shared<Magick::Image>("../images/f_test.png");
    Segmenter::Formula formula(img, {0ul, 0ul, img->columns(), img->rows()}, Segmenter::Formula::Vertical);
    formula.slice();

    img->write("../images/ans.jpg");
}
