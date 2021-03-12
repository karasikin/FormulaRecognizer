/* main.cpp */

#include <iostream> // для отладки

#include "recognizer.h"

int main(int /*argc*/, char **argv) {
    Magick::InitializeMagick(argv[0]);

    Recognizer recognizer("../images/f_test.png");
    recognizer.recognize();
}
