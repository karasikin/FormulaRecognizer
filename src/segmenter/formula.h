/* formula.h */

#ifndef FORMULA_H
#define FORMULA_H

#include <vector>
#include <Magick++.h>

#include "rect.h"

namespace Segmenter {

    class Formula {

        enum SliceDirection{ Horizontal, Vertical };

        public:

            Formula(const Magick::Image &img);

            /* Режим картинку на отдельные символы 
             * Скорее всего будем возвращать объект класса Formula 
             */
            std::vector<Magick::Image> slice();

        private:

            /* Делает разрезы по горизонтали или вертикали 
             * заданной области */
            void makeSlice(SliceDirection direction, Rect rect);

        private:

            /* изображение, которое мы будим резать */
            Magick::Image img;

            double startingConfidenceInterval;
            double endingConfidenceInterval;

    };
}

#endif
