/* formula.h */

#ifndef FORMULA_H
#define FORMULA_H

#include <vector>
#include <memory>
#include <functional>
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

            /* По-идее функция выполняется для каждого узла слева в порядке postorder.
             * В качестве аргумента functional-объекта выступает текущий узел
             */
            void postorder(std::function<void(Formula *)> f); 

            const Rect &getRectangle() const;
            bool isLeaf() const;

        private:

            /* Делает разрезы по горизонтали или вертикали 
             * заданной области */
            void makeSlice(SliceDirection direction);

            /* Добавляем сегмент в текущую  формулу */
            void addSegment(const Rect &rect);


            /* Отладочная функция рисует сегменты на исходном мзображении */
            void drawSegment(const Magick::Color &color);

            /* Отладочная функция. Создает изображение для каждого уровня вложенности сегментов */
            void makeSegmentImage(const Magick::Color &outerColor, const Magick::Color &interiorColor);

        private:

            /* Изображение, которое мы будим резать */
            std::shared_ptr<Magick::Image> img;

            /* Ограничение */
            Rect rect;

            /* Направление разреза */
            SliceDirection direction;

            /* Подформулы */
            std::vector<std::unique_ptr<Formula>> segments;
    };
}

#endif
