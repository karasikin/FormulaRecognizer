/* dataset.cpp */

#include "dataset.h"

#include <fstream>

std::ostream &operator<<(std::ostream &out, const std::vector<double> &vector) {
    for(auto &x: vector) {
        out << x << ' ';
    }

    return out;
}

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

bool Dataset::writeToFile(const std::string &fileName) const {
    std::ofstream out{fileName};

    if(!out) {
        return false;
    }

    out << sets.size() << ' ';
    if(!empty()) out << sets.front().data->size();
    out << '\n';


    for(auto &set: sets) {
        out << set.value << ' ' << *set.data << std::endl;
    }

    out.close();
    return true;
}

bool Dataset::readFromFile(const std::string &fileName) {
    std::ifstream in{fileName};

    if(!in) {
        return false;
    }

    clear();

    unsigned int setCount, setSize;
    in >> setCount >> setSize;

    for(auto i = 0ul; i < setCount; ++i) {
        int value;
        in >> value;

        auto vector_ptr = std::make_unique<std::vector<double>>();

        for(auto j = 0ul; j < setSize; ++j) {
            double item;
            in >> item;
            vector_ptr->push_back(item);
        } 

        sets.push_back({std::move(vector_ptr), value});
    }

    in.close();
    return true;
}
