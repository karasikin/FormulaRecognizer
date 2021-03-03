/* slicer.cpp */

#include <iostream>  // Для отладки
#include <cmath>
//#include <algorithm>

#include "slicer.h"

using Img = Magick::Image;

SymbolSlicer::Slicer::Slicer() {}

std::vector<Img> SymbolSlicer::Slicer::slice(Img &img) const {
    horizontalSlicing(img);

    std::vector<Img> symbols; 


    return symbols;
}

// private

void SymbolSlicer::Slicer::horizontalSlicing(Img &img) const {
    std::vector<double> fRow(img.rows());

    for(auto i = 0ul; i < img.rows(); ++i) {
        for(auto j = 0ul; j < img.columns(); ++j) {
            auto color = img.pixelColor(j, i);
            Magick::ColorGray grey(color);
            fRow[i] += grey.shade();
        }     
    }

    drawGreed(img, fRow);
    //for(auto &x: fRow) {
      //  std::cout << x << ' ';
    //}

    //std::cout << "Max: " << *std::max_element(fRow.begin(), fRow.end()) << std::endl;
    //std::cout << "Min: " << *std::min_element(fRow.begin(), fRow.end()) << std::endl;
}

void SymbolSlicer::Slicer::drawGreed(Img &img, const std::vector<double> &hSliceFunction) const {
     const size_t delta = 2;
     //double diff = (*std::max_element(hSliceFunction.begin(), hSliceFunction.end()) - 
       //     *std::min_element(hSliceFunction.begin(), hSliceFunction.end())) / 3;
    double diff = 30;

    // std::cout << "Diff: " << diff << std::endl;
    //

    auto bgSum = Magick::ColorGray(img.backgroundColor()).shade() * img.columns();
    std::cout << bgSum << std::endl;

    for(auto i = delta; i < hSliceFunction.size() - delta; ++i) {
        /*if(std::abs(hSliceFunction[i - delta] - hSliceFunction[i]) >= diff &&
                std::abs(hSliceFunction[i + delta] - hSliceFunction[i] <= diff)) 
        {
            img.draw(Magick::DrawableLine(0, i, img.rows(), i)); 
        }*/

        if(
                (std::abs(bgSum - hSliceFunction[i]) - std::abs(bgSum - hSliceFunction[i - delta])) >= diff
        ) {
            img.draw(Magick::DrawableLine(0, i, img.rows(), i)); 
        }
            

        img.pixelColor(hSliceFunction[i] - 100, i, Magick::Color("red"));
    } 
}
