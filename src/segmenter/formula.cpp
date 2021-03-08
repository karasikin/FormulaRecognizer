/* slicer.cpp */

#include <iostream>  // Для отладки

#include "formula.h"

Segmenter::Formula::Formula(const std::shared_ptr<Magick::Image> &img, 
        const Rect &rect, 
        SliceDirection direction) 
    :img(img), 
     rect(rect),
     direction(direction)
{}

void Segmenter::Formula::slice() {
    makeSlice(direction);

    /* Добавляем сегмент в нарезку 
     * нужно для наглядной отладки */
    //makeSegmentImage(Magick::Color{"red"}, Magick::Color{"green"});

    // Потомков нет
    if(segments.size() == 0) {
        /* рисуем сегмент, для отладки.
         * Нужно будет убрать */
        drawSegment(Magick::Color{"blue"});
        return;
    }

    // Потомки есть
    for(auto &segment: segments) {
        segment->slice();
    }
}

void Segmenter::Formula::postorder(std::function<void(Formula *)> f) {
    f(this); 

    for(auto &x: segments) {
        x->postorder(f);
    }
}

const Segmenter::Rect &Segmenter::Formula::getRectangle() const {
    return rect;
}

bool Segmenter::Formula::isLeaf() const {
    return segments.size() == 0;
}

// private
//

void Segmenter::Formula::makeSlice(SliceDirection direction) {
    auto rect = direction == Horizontal ? this->rect : this->rect.coup();

    auto isPeak{false}; 
    size_t xStart{};
    size_t yStart{};

    for(auto i = rect.y1; i < rect.y2; ++i) {
        auto summaryShade{0};

        for(auto j = rect.x1; j < rect.x2; ++j) {
            Magick::ColorMono color{ direction == Horizontal ? img->pixelColor(j, i) : img->pixelColor(i, j) };
            if(color.mono()) {
                summaryShade++;
            }
        }     

        // начало текстового блока
        if(!isPeak && summaryShade > 0) {
            xStart = rect.x1;
            yStart = i;
            
            isPeak = true;
        } 

        // конец блока
        if(isPeak && summaryShade <= 0) {
            addSegment(Rect{xStart, yStart, rect.x2, i});

            isPeak = false;
        }
    }

    // Защита, нужна чтобы добавлять последний элемент, уже образанного блока 
    if(isPeak && yStart != rect.y1) {
        addSegment(Rect{xStart, yStart, rect.x2, rect.y2});
    }
}

void Segmenter::Formula::addSegment(const Rect &rect) {
    if(direction == Horizontal) {
        auto segment = std::make_unique<Formula>(img, rect, Vertical);
        segments.push_back(std::move(segment));
    } else {
        auto segment = std::make_unique<Formula>(img, rect.coup(), Horizontal);
        segments.push_back(std::move(segment));
    }
}

void Segmenter::Formula::drawSegment(const Magick::Color &color) {
    img->strokeColor(color);
    img->fillColor(Magick::Color("#ffffff00"));  // Прозрачный

    img->draw(Magick::DrawableRectangle(rect.x1, rect.y1, rect.x2 - 1, rect.y2 - 1));
}

void Segmenter::Formula::makeSegmentImage(const Magick::Color &outerColor, const Magick::Color &interiorColor) {
    Magick::Image segmentImg(*img);

    segmentImg.fillColor(Magick::Color("#ffffff00")); // Прозрачный

    segmentImg.strokeColor(outerColor);
    segmentImg.draw(Magick::DrawableRectangle(rect.x1, rect.y1, rect.x2 - 1, rect.y2 - 1));

    segmentImg.strokeColor(interiorColor);
    for(auto &x: this->segments) {
        auto rect = x->getRectangle();
        segmentImg.draw(Magick::DrawableRectangle(rect.x1, rect.y1, rect.x2 - 1, rect.y2 - 1));
    }

    segmentImg.write(std::string("../images/ansset/img") 
            + std::to_string(rect.x1) 
            + std::string("x") 
            + std::to_string(rect.y1)
            + std::string("x") 
            + std::to_string(rect.x2) 
            + std::string("x") 
            + std::to_string( rect.y2) 
            + std::string(".png"));
}
