/* datasetbuilder.h */

#ifndef DATASETBUILDER_H
#define DATASETBUILDER_H

#include <fstream>
#include <vector>

#include <QWidget>

class QLabel;
class QLineEdit;
class QPushButton;

class DatasetBuilder : public QWidget {
    Q_OBJECT

    struct Data {
        std::unique_ptr<std::vector<double>> data;
        int value;
    };

    public:

        DatasetBuilder(QWidget *parent = nullptr);

    private slots:

        void onDataLoad();

    private:

        std::fstream datasetFile;

        std::vector<Data> data;

        QLabel *datasetImg;
        QLineEdit *datasetLineEdit;
        QPushButton *datasetLoadBtn;
        QPushButton *datasetDeleteBtn;
        QPushButton *datasetNextBtn;
        QPushButton *datasetPrevBtn;

        QLabel *dataImg;
        QLineEdit *dataLineEdit;
        QPushButton *dataLoadBtn;
        QPushButton *dataAddBtn;
        QPushButton *dataNextBtn;
        QPushButton *dataPrevBtn;
};

#endif
