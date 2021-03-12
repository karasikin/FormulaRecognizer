/* slicer.cpp */

#include <iostream>  // Для отладки

#include "formula.h"

Segmenter::Formula::Formula(
        const std::shared_ptr<Magick::Image> &grayImg, 
        const std::shared_ptr<Magick::Image> &monoImg,
        const Rect &rect, 
        SliceDirection direction
)
    :SLICE_SIDE_SIZE(32),
     grayImg(grayImg), 
     monoImg(monoImg),
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

std::string Segmenter::Formula::toLaTeX() const {
    if(isLeaf()) {
        std::vector<double> sliceVector;

        std::string prefix{"../images/ansset/img"};

        if((rect.x2 - rect.x1) > SLICE_SIDE_SIZE && (rect.y2 - rect.y1) > SLICE_SIDE_SIZE) {
            sliceVector = toVectorWithCompression(SLICE_SIDE_SIZE);
            prefix+= "_compression_";
        } else {
            sliceVector = toVectorWithStretch(SLICE_SIDE_SIZE);
            prefix += "_stretched_";
        }

        // Для отладки!
        //makeImageFromVector(sliceVector, "../images/ansset/img"); 
        makeImageFromVector(sliceVector, prefix); 

        /// Получаем строку
    }

    for(auto &x: segments) {
        x->toLaTeX();        
    }

    return {}; // Возвращаем строку
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
            Magick::ColorMono color{ direction == Horizontal ? monoImg->pixelColor(j, i) : monoImg->pixelColor(i, j) };
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
        auto segment = std::make_unique<Formula>(grayImg, monoImg, rect, Vertical);
        segments.push_back(std::move(segment));
    } else {
        auto segment = std::make_unique<Formula>(grayImg, monoImg, rect.coup(), Horizontal);
        segments.push_back(std::move(segment));
    }
}

std::vector<double> Segmenter::Formula::toVectorWithCompression(size_t sideSize) const {
    std::vector<double> sliceVector(sideSize * sideSize); 

    auto xk = (rect.x2 - rect.x1) / double(sideSize);
    auto yk = (rect.y2 - rect.y1) / double(sideSize);

    // Если изображение сжимается 
    for(auto i = rect.y1; i < rect.y2; ++i) {
        for(auto j = rect.x1; j < rect.x2; ++j) {
            auto color = Magick::ColorGray{grayImg->pixelColor(j, i)};

            auto _j = int((j - rect.x1) / xk);
            auto _i = int((i - rect.y1) / yk);

            sliceVector[_i * sideSize + _j] = color.shade();
        }
    }
    
    return sliceVector;
}

std::vector<double> Segmenter::Formula::toVectorWithStretch(size_t sideSize) const {
    std::vector<double> sliceVector(sideSize * sideSize);

    auto xk = (rect.x2 - rect.x1) / double(sideSize);
    auto yk = (rect.y2 - rect.y1) / double(sideSize);

    for(auto i = 0ul; i < sideSize; ++i) {
        for(auto j = 0ul; j < sideSize; ++j) {
            auto _j = int(j * xk) + rect.x1;
            auto _i = int(i * yk) + rect.y1;

            auto color = Magick::ColorGray(grayImg->pixelColor(_j, _i));

            sliceVector[i * sideSize + j] = color.shade();
        }
    }

    return sliceVector;
}

void Segmenter::Formula::drawSegment(const Magick::Color &color) {
    monoImg->strokeColor(color);
    monoImg->fillColor(Magick::Color("#ffffff00"));  // Прозрачный

    monoImg->draw(Magick::DrawableRectangle(rect.x1, rect.y1, rect.x2 - 1, rect.y2 - 1));
}

void Segmenter::Formula::makeSegmentImage(const Magick::Color &outerColor, const Magick::Color &interiorColor) {
    Magick::Image segmentImg(*monoImg);

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

void Segmenter::Formula::makeImageFromVector(const std::vector<double> &vector, const std::string &prefix) const {
    Magick::Image img;
    img.size({SLICE_SIDE_SIZE, SLICE_SIDE_SIZE});

    for(auto i = 0ul; i < vector.size(); ++i) {
        img.pixelColor(i % SLICE_SIDE_SIZE, i / SLICE_SIDE_SIZE, Magick::ColorGray(vector[i]));
    }

    img.write(prefix
            + std::to_string(rect.x1) 
            + std::string("x") 
            + std::to_string(rect.y1)
            + std::string("x") 
            + std::to_string(rect.x2) 
            + std::string("x") 
            + std::to_string( rect.y2) 
            + std::string(".png"));
}
