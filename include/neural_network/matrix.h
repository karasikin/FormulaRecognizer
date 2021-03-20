/* matrix.h */

#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <ostream>

namespace NeuralNetwork {

    class Matrix {

        public:

            Matrix(); 

            size_t rows() const;
            size_t columns() const;

            void generate(size_t rows, size_t columns, double lowerBorder, double upperBorder);

            std::vector<double> operator*(const std::vector<double> &vectorColumn) const;

        private:

            std::vector<std::vector<double>> matrix;

            friend std::ostream &operator<<(std::ostream &out, const Matrix &matrix);
    };

    std::ostream &operator<<(std::ostream &out, const Matrix &matrix);
}


#endif // MATRIX_H
