/* maintrix.cpp */

#include "neural_network/matrix.h"

#include <random>
#include <iomanip>

NeuralNetwork::Matrix::Matrix() 
    :matrix()
{}

size_t NeuralNetwork::Matrix::rows() const {
    return matrix.size();
}

size_t NeuralNetwork::Matrix::columns() const {
    return matrix.size() == 0 ? 0 : matrix.front().size();
}

void NeuralNetwork::Matrix::generate(size_t rows, size_t columns, double lowerBorder, double upperBorder) {
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution uid{lowerBorder, upperBorder};

    matrix.resize(rows);
    for(auto &row: matrix) {
        row.resize(columns); 
        for(auto &item: row) {
            item = uid(gen);
        }
    }
}

std::vector<double> NeuralNetwork::Matrix::operator*(const std::vector<double> &vectorColumn) const {
    
    if(vectorColumn.size() != columns()) {
        return {};
    }

    std::vector<double> vectorResult(rows());

    for(auto i = 0ul; i < rows(); ++i) {
        for(auto j = 0ul; j < columns(); ++j) {
            vectorResult[i] += matrix[i][j] * vectorColumn[j]; 
        }
    }

    return vectorResult;
}

std::ostream &NeuralNetwork::operator<<(std::ostream &out, const Matrix &matrix) {
    out << std::setprecision(5);
    for(auto &row: matrix.matrix) {
        for(auto &item: row) {
            out << item << ' ';
        }
        out << std::endl;
    }

    return out;
}
