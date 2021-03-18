/* datasetbuilder.h */

#ifndef DATASETBUILDER_H
#define DATASETBUILDER_H

#include <fstream>
#include <vector>

#include <QWidget>

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
        void onDataNext();
        void onDataPrev();

    private:

        void showData(const Data &data);
        void extractData(const std::string &fileName);

    private:

        //std::fstream datasetFile;

        std::vector<Data> data;
        int currentData;

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
