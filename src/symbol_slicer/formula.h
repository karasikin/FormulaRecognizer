/* formula.h */

/* Идея в том, что формула состоит из подформул,
 * следовательно можно в разбить всю картинку на регионы,
 * в каждом из которых будут свои регионы. 
 * Между регионами в одной формулы сушествуют некоторые связи.
 * Например знак отношения 
 *      a + b
 *      -----
 *        c
 * в этом примере формула содержит две подформулы a+b и c 
 *
 * Возможно формула будет привязанна к како-то картинке!!!
 *
 */

#ifndef FORMULA_H
#define FORMULA_H

#include <Magick++.h>

#include "rect.h"


namespace SymbolSlicer {

    class Formula {
    
        public:
            Formula(const Magick::Image &img, const Rect &rect);

        private:
            const Magick::Image &img;   // Подумать очень сильно подумать!!!

            Rect rect;
    };
}

#endif
