/* slicer.cpp */

#include <iostream>  // Для отладки
#include <cmath>
//#include <algorithm>
#include <random>

#include "formula.h"

using Img = Magick::Image;

Segmenter::Formula::Formula(const std::shared_ptr<Magick::Image> &img, 
        const Rect &rect, 
        SliceDirection direction) 
    :img(img), 
     rect(rect),
     direction(direction),
     startingConfidenceInterval(1.0),
     endingConfidenceInterval(1.0) 
{}

void Segmenter::Formula::slice() {
    
    std::cout << "Slice: ";

    makeSlice(direction);
        drawSegment();

    if(segments.size() == 1 && segments.front()->getRectangle() == rect) {
        // Потомок единственный и он такой же как отец
        std::cout << "Потомок такойже return" << std::endl;
        drawSegment();
        return;
    } 

    if(segments.size() == 0) {
        // Потомков нет
        std::cout << "Потомков нет return" << std::endl;
        return;
    }

    // Потомки есть
    std::cout << "Потомки есть обработка" << std::endl;
    for(auto &segment: segments) {
        segment->slice();
    }
}

Segmenter::Rect Segmenter::Formula::getRectangle() const {
    return rect;
}

// private
//

void Segmenter::Formula::makeSlice(SliceDirection direction) {
    if(direction == Vertical) {
        rect = rect.coup();
    }

    auto summaryBgShade{Magick::ColorGray(img->backgroundColor()).shade() * rect.x2 - rect.x1};

    /* На данный момент будет работать только для светлого фона.
     * Подумать над темным фоном */
    auto allowedStartingShade{summaryBgShade - startingConfidenceInterval}; 
    auto allowedEndingShade{summaryBgShade - endingConfidenceInterval}; 

    auto isPeak{false}; 

    size_t xStart{};
    size_t yStart{};

    for(auto i = rect.y1; i < rect.y2; ++i) {
        auto summaryShade{0.};

        for(auto j = rect.x1; j < rect.x2; ++j) {
            auto color = direction == Horizontal ? img->pixelColor(j, i) : img->pixelColor(i, j);
            Magick::ColorGray grey(color);
            summaryShade += grey.shade();
        }     

        // начало текстового блока
        if(!isPeak && summaryShade < allowedStartingShade) {
            xStart = rect.x1;
            yStart = i;
            
            isPeak = true;
        } 

        // конец блока
        if(isPeak && summaryShade > allowedEndingShade) {
            Rect segmentRect{xStart, yStart, rect.x2, i};
            if(direction == Horizontal) {
                auto segment = std::make_unique<Formula>(img, segmentRect, Vertical);
                segments.push_back(std::move(segment));
            } else {
                auto segment = std::make_unique<Formula>(img, segmentRect.coup(), Horizontal);
                segments.push_back(std::move(segment));
            }

            isPeak = false;
        }
    }
}

void Segmenter::Formula::drawSegment() {
    /*std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_real_distribution udd(0.1, 0.99);

    Magick::ColorRGB color(udd(gen), udd(gen), udd(gen));
    */

    Magick::Color color("red");

    img->strokeColor(color);
    img->fillColor(Magick::Color("#ffffff00"));

    img->draw(Magick::DrawableRectangle(rect.x1, rect.y1, rect.x2 - 1, rect.y2 - 1));
}

