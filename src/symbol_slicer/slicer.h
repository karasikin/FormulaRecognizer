/* slicer.h */

#ifndef SLICER_H
#define SLICER_H

#include <vector>
#include <Magick++.h>

namespace SymbolSlicer {

    class Slicer {

        enum SliceDirection{ Horizontal, Vertical };

        struct Rect {
            size_t x1, y1, x2, y2;

            Rect coup();
        };

        public:
            Slicer(const Magick::Image &img);

            /* Режим картинку на отдельные символы 
             * Скорее всего будем возвращать объект класса Formula 
             */
            std::vector<Magick::Image> slice();

        private:

            void makeSlice(SliceDirection direction, Rect rect);

        private:

            /* изображение, которое мы будим резать */
            Magick::Image img;

            double startingConfidenceInterval;
            double endingConfidenceInterval;


    };

}

#endif
