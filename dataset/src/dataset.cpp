/* dataset.cpp */

#include "dataset.h"

Dataset::Dataset() 
    :sets(),
     currentSet(-1)
{}

std::vector<double> &Dataset::operator[](size_t index) const {
    return (*sets[index].data);
}

int Dataset::getValueAtIndex(size_t index) const {
    return sets[index].value;
}

void Dataset::setValueAtIndex(size_t index, int value) {
    sets[index].value = value;
}

void Dataset::setVectorAtIndex(size_t index, std::unique_ptr<std::vector<double>> data) {
    sets[index].data = std::move(data); 
}

int Dataset::getCurrentIndex() const {
    return currentSet;
}

void Dataset::setCurrentIndex(int value) {
    currentSet = value;
}

const Dataset &Dataset::operator++() {
    ++currentSet;
    return *this;
}

const Dataset &Dataset::operator--() {
    --currentSet;
    return *this;
}
