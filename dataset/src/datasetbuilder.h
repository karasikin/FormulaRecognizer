/* datasetbuilder.h */

#ifndef DATASETBUILDER_H
#define DATASETBUILDER_H

#include <vector>

#include <QWidget>

#include "dataset.h"

class QLabel;
class QLineEdit;
class QPushButton;
class ImageViewer;

class DatasetBuilder : public QWidget {
    Q_OBJECT

    struct Data {
        std::unique_ptr<std::vector<double>> data;
        int value;
    };

    public:

        const unsigned int COLOR_COUNT; 

        DatasetBuilder(QWidget *parent = nullptr);

    private slots:

        void onDataLineEdit();
        void onDataLoad();
        void onDataAdd();
        void onDataNext();
        void onDataPrev();

        void onDatasetLoad();

    private:

        void showData(const Dataset &data);
        void extractData(const std::string &fileName);

    private:

        Dataset imageData;
        //Dataset datasetData;


        /* Элементы интерфейса */
        ImageViewer *datasetViewer;
        QLineEdit *datasetLineEdit;
        QPushButton *datasetLoadBtn;
        QPushButton *datasetDeleteBtn;
        QPushButton *datasetNextBtn;
        QPushButton *datasetPrevBtn;

        ImageViewer *dataViewer;
        QLineEdit *dataLineEdit;
        QPushButton *dataLoadBtn;
        QPushButton *dataAddBtn;
        QPushButton *dataNextBtn;
        QPushButton *dataPrevBtn;
};

#endif
