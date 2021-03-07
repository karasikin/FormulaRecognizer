/* main.cpp */

#include <iostream> // для отладки

#include "segmenter/formula.h"

std::vector<std::vector<bool>> binaryzation(Magick::Image &img) {
    std::vector<std::vector<bool>> binaryImg{img.rows()};
    for(auto i = 0ul; i < img.columns(); ++i) {
        binaryImg.resize(img.columns());
    }

    for(auto i = 0ul; i < img.rows(); ++i) {
        for(auto j = 0ul; j < img.columns(); ++j) {
            auto color = img.pixelColor(j, i);
            Magick::ColorGray colorGray(color);
            if(colorGray.shade() < 0.90) {
                //binaryImg[j][i] = true;
                img.pixelColor(j, i, Magick::ColorGray(0.0));
            }
        }
    }

    return binaryImg;
}

int main(int /*argc*/, char **argv) {
    Magick::InitializeMagick(argv[0]);


    auto img = std::make_shared<Magick::Image>("../images/f_test.png");
    img->type(Magick::ImageType::BilevelType);

    Segmenter::Formula formula(img, {0ul, 0ul, img->columns(), img->rows()}, 
            Segmenter::Formula::Vertical);
    formula.slice();

    img->write("../images/ans.jpg");
}
