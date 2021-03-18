/* dataset.h */

#ifndef DATASET_H
#define DATASET_H

#include <vector>
#include <memory>

std::ostream &operator>>(std::ostream &out, const std::vector<double> &vector);

class Dataset {

    public: struct Data {
        std::unique_ptr<std::vector<double>> data;
        int value;
    };

    public:

        Dataset();

        double &operator[](size_t index) const;

        int getValue() const;
        void setValue(int value);

        void add(std::unique_ptr<std::vector<double>> data);
        void clear();

        const Dataset &operator++();
        const Dataset &operator--();

        bool empty() const;
        size_t currentSetSize() const;

        bool isFirst() const;
        bool isLast() const;

        bool writeToFile(const std::string &fileName) const;
        bool readFromFile(const std::string &fileName);

    private:
        
        std::vector<Data> sets; 
        int currentSetIndex;

};

#endif // DATASET_H
