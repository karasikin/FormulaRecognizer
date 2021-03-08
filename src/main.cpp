/* main.cpp */

#include <iostream> // для отладки

#include "segmenter/formula.h"

int main(int /*argc*/, char **argv) {
    Magick::InitializeMagick(argv[0]);

    auto img = std::make_shared<Magick::Image>("../images/text.jpg");
    img->type(Magick::ImageType::BilevelType);

    Segmenter::Formula formula(img, {0ul, 0ul, img->columns(), img->rows()}, 
            Segmenter::Formula::Horizontal);
    formula.slice();

    auto f = [](Segmenter::Formula *node) {
        if(node->isLeaf()) {
            return;
        }

        auto rect = node->getRectangle();
        std::cout << "{" 
            << rect.x1 << ", " << rect.y1 << ", " 
            << rect.x2 << ", " << rect.y2
            << "}" << std::endl;
    };

    formula.postorder(f);

    img->write("../images/ans.jpg");
}
