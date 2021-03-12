/* recognizer.h */

#ifndef RECOGNIZER_H
#define RECOGNIZER_H

#include "segmenter/formula.h"

class Recognizer {

    public:

        Recognizer(const std::string &fileName);

        /* Должен возвращать LaTeX строку */
        std::string recognize();

    private:

    //    std::vector<double> sliceToArray(const Segmenter::Rect &rect, size_t sideSize) const;

    private:

        std::shared_ptr<Magick::Image> grayImg;
        std::shared_ptr<Magick::Image> monoImg;

        std::unique_ptr<Segmenter::Formula> formula;
};


#endif // RECOGNIZER_H
