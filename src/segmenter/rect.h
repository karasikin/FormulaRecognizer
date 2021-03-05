/* rect.h */

/* Вспомогательная структура 
 * Просто тупо прямоугольник
 * 
 * -----------------------> x
 * | (x1, y1)
 * |   -------
 * |   |     |
 * |   |     |
 * |   |     |
 * |   |_____|
 * |        (x2, y2)
 * \/
 *  y
 *
 * */

#ifndef RECT_H
#define RECT_h

#include <cstddef>

namespace Segmenter {

    struct Rect {
        size_t x1, y1, x2, y2;

        Rect coup() const;
    };

}

#endif
