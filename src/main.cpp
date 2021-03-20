/* main.cpp */

#include <iostream> // для отладки

#include "recognizer.h"
#include "neural_network/matrix.h"

int main(int /*argc*/, char **argv) {
    Magick::InitializeMagick(argv[0]);

    // Recognizer recognizer("../images/f_test.jpg");
    // recognizer.recognize();

    NeuralNetwork::Matrix matrix1;
    matrix1.generate(4, 3, 1.0, 5.0);
    std::cout << matrix1 << std::endl;

    std::vector<double> v1{1.1, 2.1, 3.1};

    for(auto &item: matrix1 * v1) {
        std::cout << item << ' ';
    } std::cout << std::endl;
}
