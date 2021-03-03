/* slicer.h */

#ifndef SLICER_H
#define SLICER_H

#include <vector>
#include <Magick++.h>

namespace SymbolSlicer {

    class Slicer {

        public:
            Slicer();

            /* Режим картинку на отдельные символы 
             * Скорее всего будем возвращать объект класса Formula 
             */
            std::vector<Magick::Image> slice(Magick::Image &img) const;

        private:

            /* Режим картинку по горизонтали */
            void horizontalSlicing(Magick::Image &img) const;

            /* Временная тестовая функция
             * рисует на изображении сетку в тех местах, где должно 
             * быть разбиение. Предполагает, что все символы одинакового размера и располложены
             * в рядами 
             */
            void drawGreed(Magick::Image &img, const std::vector<double> &hSliceFunction) const;

    };

}

#endif
