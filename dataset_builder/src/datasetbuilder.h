/* datasetbuilder.h */

#ifndef DATASETBUILDER_H
#define DATASETBUILDER_H

#include <vector>

#include <QWidget>

#include "dataset/dataset.h"

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
        const int VIEWER_SIZE;
        const int SEGMENT_SIDE_SIZE;

        DatasetBuilder(QWidget *parent = nullptr);

    private slots:

        void onDataLineEdit(const QString &text);
        void onDataLoad();
        void onDataAdd();
        void onDataNext();
        void onDataPrev();

        void onDatasetLineEdit(const QString &text);
        void onDatasetLoad();
        void onDatasetDelete();
        void onDatasetSave();
        void onDatasetNext();
        void onDatasetPrev();

    private:

        void showData(const Dataset::Dataset &data, ImageViewer *imgViewer, QLineEdit *lineEdit);
        void extractData(const std::string &fileName);

    private:

        Dataset::Dataset imageData;
        Dataset::Dataset datasetData;

        /* Элементы интерфейса */
        ImageViewer *datasetViewer;
        QLineEdit *datasetLineEdit;
        QPushButton *datasetLoadBtn;
        QPushButton *datasetDeleteBtn;
        QPushButton *datasetSaveBtn;
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
