/* dataset.h */

#ifndef DATASET_H
#define DATASET_H

#include <vector>
#include <memory>

class Dataset {

    public: struct Data {
        std::unique_ptr<std::vector<double>> data;
        int value;
    };

    public:

        Dataset();

        std::vector<double> &operator[](size_t index) const;

        int getValueAtIndex(size_t index) const;
        void setValueAtIndex(size_t index, int value);

        void setVectorAtIndex(size_t index, std::unique_ptr<std::vector<double>> data);

        int getCurrentIndex() const;
        void setCurrentIndex(int value);
        const Dataset &operator++();
        const Dataset &operator--();

    private:
        
        std::vector<Data> sets; 
        int currentSet;

};

#endif // DATASET_H
