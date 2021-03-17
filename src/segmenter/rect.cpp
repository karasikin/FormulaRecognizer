/* rect.cpp */

#include "segmenter/rect.h"

bool Segmenter::Rect::operator==(const Rect &other) {
    return x1 == other.x1 
        && y1 == other.y1 
        && x2 == other.x2 
        && y2 == other.y2;
}

Segmenter::Rect Segmenter::Rect::coup() const {
    return {y1, x1, y2, x2};
}
