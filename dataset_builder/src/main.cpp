/* main.cpp */

#include <QApplication>

#include "datasetbuilder.h"

int main(int argc, char **argv) {
    QApplication app{argc, argv};

    DatasetBuilder datasetBuilder{}; 
    datasetBuilder.show();

    return app.exec();
}
