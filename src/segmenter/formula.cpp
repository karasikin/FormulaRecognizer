/* slicer.cpp */

#include <iostream>  // Для отладки
#include <cmath>
//#include <algorithm>

#include "formula.h"

using Img = Magick::Image;

Segmenter::Formula::Formula(const Img &img) 
    :img(std::move(img)), startingConfidenceInterval(1.0),
     endingConfidenceInterval(1.0) 
{}

std::vector<Img> Segmenter::Formula::slice() {
    Rect r{0ul, 0ul, img.columns(), img.rows()};

    //horizontalSlicing({0ul, img.rows() / 2, img.columns(), img.rows()});
    makeSlice(Horizontal, r);
    //horizontalSlicing({0ul, 0ul, img.rows(), img.columns()});

    std::vector<Img> symbols; 


    img.write("../images/ans.jpg");

    return symbols;
}

// private
//

void Segmenter::Formula::makeSlice(SliceDirection direction, Rect rect) {
    if(direction == Vertical) {
        rect = rect.coup();
    }

    auto summaryBgShade{Magick::ColorGray(img.backgroundColor()).shade() * rect.x2};

    /* На данный момент будет работать только для светлого фона.
     * Подумать над темным фоном */
    auto allowedStartingShade{summaryBgShade - startingConfidenceInterval}; 
    auto allowedEndingShade{summaryBgShade - endingConfidenceInterval}; 

    auto isPeak{false}; 

    for(auto i = rect.y1; i < rect.y2; ++i) {
        auto summaryShade{0.};

        for(auto j = rect.x1; j < rect.x2; ++j) {
            auto color = direction == Horizontal ? img.pixelColor(j, i) : img.pixelColor(i, j);
            Magick::ColorGray grey(color);
            summaryShade += grey.shade();
        }     


        if(!isPeak && summaryShade < allowedStartingShade) {
            // начало текстового блока
            std::cout << "was started" << std::endl;

            if(direction == Horizontal) {
                img.draw(Magick::DrawableLine(0, i, img.columns(), i)); 
            } else {
                img.draw(Magick::DrawableLine(i, 0, i, img.columns())); 
            }
            
            isPeak = true;
        } 

        if(isPeak && summaryShade > allowedEndingShade) {
            // конец блока
            std::cout << "was ended" << std::endl;
            
            if(direction == Horizontal) {
                img.draw(Magick::DrawableLine(0, i, img.columns(), i)); 
            } else {
                img.draw(Magick::DrawableLine(i, 0, i, img.columns())); 
            }

            isPeak = false;
        }

        /* Для прорисовки графика */
        //img.pixelColor(summaryShade - 10, i, Magick::Color("red"));
    }
}

