/* formula.h */

#ifndef FORMULA_H
#define FORMULA_H

#include <vector>
#include <memory>
#include <Magick++.h>

#include "rect.h"

namespace Segmenter {

    class Formula {

        public:
            enum SliceDirection{ Horizontal, Vertical };

            Formula(const std::shared_ptr<Magick::Image> &img, const Rect &rect, 
                    SliceDirection direction);

            /* Режим картинку на отдельные символы 
             * Скорее всего будем возвращать объект класса Formula 
             */
            void slice();

            Rect getRectangle() const;

        private:

            /* Делает разрезы по горизонтали или вертикали 
             * заданной области */
            void makeSlice(SliceDirection direction);

            /* !!!! Отладочная функция рисует сегменты на исходном мзображении */
            void drawSegment();

        private:

            /* Изображение, которое мы будим резать */
            std::shared_ptr<Magick::Image> img;

            /* Ограничение */
            Rect rect;

            /* Направление разреза */
            SliceDirection direction;

            /* Подформулы */
            std::vector<std::unique_ptr<Formula>> segments;

            double startingConfidenceInterval;
            double endingConfidenceInterval;

    };
}

#endif
