/* dataset.cpp */

#include "dataset.h"

Dataset::Dataset() 
    :sets(),
     currentSetIndex(-1)
{}

double &Dataset::operator[](size_t index) const {
    return (*sets[currentSetIndex].data)[index];
}

int Dataset::getValue() const {
    return sets[currentSetIndex].value;
}

void Dataset::setValue(int value) {
    sets[currentSetIndex].value = value;
}

void Dataset::add(std::unique_ptr<std::vector<double>> data) {
    sets.push_back({std::move(data), 0});
}

void Dataset::clear() {
    currentSetIndex = -1;
    sets.clear();
}

const Dataset &Dataset::operator++() {
    ++currentSetIndex;
    return *this;
}

const Dataset &Dataset::operator--() {
    --currentSetIndex;
    return *this;
}

bool Dataset::empty() const {
    return sets.empty();
}

size_t Dataset::currentSetSize() const {
    return sets[currentSetIndex].data->size(); 
}

bool Dataset::isFirst() const {
    return currentSetIndex == 0;
}

bool Dataset::isLast() const {
    return currentSetIndex == int(sets.size() - 1);
}
