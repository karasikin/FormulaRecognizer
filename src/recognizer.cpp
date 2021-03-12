/* recognizer.cpp */

#include "recognizer.h"

Recognizer::Recognizer(const std::string &fileName) {
    grayImg = std::make_shared<Magick::Image>(fileName);
    grayImg->type(Magick::GrayscaleType);

    monoImg = std::make_shared<Magick::Image>(*grayImg);
    monoImg->type(Magick::BilevelType);

    formula = std::make_unique<Segmenter::Formula>(
            grayImg, 
            monoImg,
            Segmenter::Rect{0ul, 0ul, monoImg->columns(), monoImg->rows()}, 
            Segmenter::Formula::Horizontal
        );
}

std::string Recognizer::recognize() {
    formula->slice();
    formula->toLaTeX();

    monoImg->write("../images/ans.jpg");

    return "Not Implemented";
}
