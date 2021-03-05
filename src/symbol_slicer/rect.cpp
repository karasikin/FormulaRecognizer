/* rect.cpp */

#include "rect.h"

SymbolSlicer::Rect SymbolSlicer::Rect::coup() const {
    return {y1, x1, y2, x2};
}
