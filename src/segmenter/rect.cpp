/* rect.cpp */

#include "rect.h"

Segmenter::Rect Segmenter::Rect::coup() const {
    return {y1, x1, y2, x2};
}
