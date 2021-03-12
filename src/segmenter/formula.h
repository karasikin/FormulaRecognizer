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

            const size_t SLICE_SIDE_SIZE;

            Formula(const std::shared_ptr<Magick::Image> &grayImg, 
                    const std::shared_ptr<Magick::Image> &monoImg,
                    const Rect &rect, 
                    SliceDirection direction);

            /* Режим картинку на отдельные символы 
             * Скорее всего будем возвращать объект класса Formula 
             */
            void slice();

            /* Переводим картинку в LateX ? */
            std::string toLaTeX() const;

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

            /* Преобразуем формулу в std::vector<double> 
             * Работает корректно, кодда размер получаемой матрицы МЕНЬШЕ,
             * чем размер исходного изображения */
            std::vector<double> toVectorWithCompression(size_t sideSize) const;

            /* Преобразуем формулу в std::vector<double> 
             * Работает корректно, кодда размер получаемой матрицы БОЛЬШЕ,
             * чем размер исходного изображения */
            std::vector<double> toVectorWithStretch(size_t sideSize) const;


            /* Отладочная функция рисует сегменты на исходном мзображении */
            void drawSegment(const Magick::Color &color);

            /* Отладочная функция. Создает изображение для каждого уровня вложенности сегментов */
            void makeSegmentImage(const Magick::Color &outerColor, const Magick::Color &interiorColor);

            /* Отладочная функция рисует изображение по полученному для сегмента вектору */
            void makeImageFromVector(const std::vector<double> &vector, const std::string &prefix) const;

        private:

            /* Изображение, которое мы будим резать */
            std::shared_ptr<Magick::Image> grayImg;
            std::shared_ptr<Magick::Image> monoImg;

            /* Ограничение */
            Rect rect;

            /* Направление разреза */
            SliceDirection direction;

            /* Подформулы */
            std::vector<std::unique_ptr<Formula>> segments;
    };
}

#endif
