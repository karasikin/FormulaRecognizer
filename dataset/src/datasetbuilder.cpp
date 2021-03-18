/* datasetbuilder.cpp */

#include "datasetbuilder.h"

#include <cmath>

#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>

#include "segmenter/formula.h"
#include "imageviewer.h"

#include <QDebug>


DatasetBuilder::DatasetBuilder(QWidget *parent) 
    :QWidget(parent),
     COLOR_COUNT(256),
     imageData(),
     datasetData()
{
    datasetViewer = new ImageViewer;
    datasetLineEdit = new QLineEdit;
    datasetLoadBtn = new QPushButton("Load");
    datasetDeleteBtn = new QPushButton("Delete");
    datasetPrevBtn = new QPushButton("<");
    datasetNextBtn = new QPushButton(">");

    dataViewer = new ImageViewer;
    dataLineEdit = new QLineEdit;
    dataLoadBtn = new QPushButton("Load");
    dataAddBtn = new QPushButton("Add");
    dataPrevBtn = new QPushButton("<");
    dataNextBtn = new QPushButton(">");

    datasetViewer->setMinimumWidth(100);  // ?

    dataViewer->setMinimumWidth(100); // ?
    dataNextBtn->setEnabled(false);
    dataPrevBtn->setEnabled(false);

    auto datasetNavigationLayout = new QHBoxLayout;
    datasetNavigationLayout->addWidget(datasetPrevBtn);
    datasetNavigationLayout->addWidget(datasetNextBtn);

    auto datasetActionLayout = new QVBoxLayout;
    datasetActionLayout->addWidget(datasetLineEdit);
    datasetActionLayout->addWidget(datasetLoadBtn);
    datasetActionLayout->addWidget(datasetDeleteBtn);

    auto datasetTopLayout = new QHBoxLayout;
    datasetTopLayout->addWidget(datasetViewer);
    datasetTopLayout->addLayout(datasetActionLayout);

    auto datasetLayout = new QVBoxLayout;
    datasetLayout->addLayout(datasetTopLayout);
    datasetLayout->addLayout(datasetNavigationLayout);

    auto dataNavigationLayout = new QHBoxLayout;
    dataNavigationLayout->addWidget(dataPrevBtn);
    dataNavigationLayout->addWidget(dataNextBtn);

    auto dataActionLayout = new QVBoxLayout;
    dataActionLayout->addWidget(dataLineEdit);
    dataActionLayout->addWidget(dataLoadBtn);
    dataActionLayout->addWidget(dataAddBtn);

    auto dataTopLayout = new QHBoxLayout;
    dataTopLayout->addWidget(dataViewer);
    dataTopLayout->addLayout(dataActionLayout);

    auto dataLayout = new QVBoxLayout;
    dataLayout->addLayout(dataTopLayout);
    dataLayout->addLayout(dataNavigationLayout);

    auto *layout = new QHBoxLayout;
    layout->addLayout(dataLayout);
    layout->addLayout(datasetLayout);

    setLayout(layout);

    QObject::connect(dataLineEdit, &QLineEdit::textChanged, this, &DatasetBuilder::onDataLineEdit);
    QObject::connect(dataLoadBtn, &QPushButton::clicked, this, &DatasetBuilder::onDataLoad);
    QObject::connect(dataAddBtn, &QPushButton::clicked, this, &DatasetBuilder::onDataAdd);
    QObject::connect(dataNextBtn, &QPushButton::clicked, this, &DatasetBuilder::onDataNext);
    QObject::connect(dataPrevBtn, &QPushButton::clicked, this, &DatasetBuilder::onDataPrev);

    QObject::connect(datasetLoadBtn, &QPushButton::clicked, this, &DatasetBuilder::onDatasetLoad);
    QObject::connect(datasetNextBtn, &QPushButton::clicked, this, &DatasetBuilder::onDatasetNext);
    QObject::connect(datasetPrevBtn, &QPushButton::clicked, this, &DatasetBuilder::onDatasetPrev);
}

void DatasetBuilder::onDataLineEdit() {
    if(!imageData.empty()) {
        imageData.setValue(dataLineEdit->text().toInt());
    }
}

void DatasetBuilder::onDataLoad() {
    auto fileName = QFileDialog::getOpenFileName(this, "Open image", ".", "Images (*.png *.jpg)").toStdString();

    if(fileName.empty()) {
        return;
    }

    imageData.clear();
    dataNextBtn->setEnabled(false);
    dataPrevBtn->setEnabled(false);

    extractData(fileName);

    if(!imageData.empty()) {
        showData(++imageData, dataViewer, dataLineEdit);
        if(!imageData.isLast()) {
            dataNextBtn->setEnabled(true);
        }
    }
}

void DatasetBuilder::onDataAdd() {
    /*imageData.writeToFile("file.dst");
    imageData.readFromFile("file.dst");
    showData(++imageData);*/
}

void DatasetBuilder::onDataNext() {
    showData(++imageData, dataViewer, dataLineEdit);

    if(!imageData.isFirst()) {
        dataPrevBtn->setEnabled(true);
    }

    if(imageData.isLast()) {
        dataNextBtn->setEnabled(false);
    }
}

void DatasetBuilder::onDataPrev() {
    showData(--imageData, dataViewer, dataLineEdit);

    if(!imageData.isLast()) {
        dataNextBtn->setEnabled(true);
    }

    if(imageData.isFirst()) {
        dataPrevBtn->setEnabled(false);
    }
}

void DatasetBuilder::onDatasetLoad() {
    auto fileName = QFileDialog::getOpenFileName(this, "Open File", ".", "Datasets (*.dst)").toStdString(); 

    if(fileName.empty()) {
        return;
    }

    datasetData.readFromFile(fileName); 

    datasetNextBtn->setEnabled(false);
    datasetPrevBtn->setEnabled(false);

    if(!datasetData.empty()) {
        showData(++datasetData, datasetViewer, datasetLineEdit); 
        if(!datasetData.isLast()) {
            datasetNextBtn->setEnabled(true);
        }
    }
}

void DatasetBuilder::onDatasetNext() {
    showData(++datasetData, datasetViewer, datasetLineEdit);

    if(datasetData.isLast()) {
        datasetNextBtn->setEnabled(false);
    }

    if(!datasetData.isFirst()) {
        datasetPrevBtn->setEnabled(true);
    }
}

void DatasetBuilder::onDatasetPrev() {
    showData(--datasetData, datasetViewer, datasetLineEdit);

    if(datasetData.isFirst()) {
        datasetPrevBtn->setEnabled(false);
    }

    if(!datasetData.isLast()) {
        datasetNextBtn->setEnabled(true);
    }
}

void DatasetBuilder::showData(const Dataset &data, ImageViewer *imgViewer, QLineEdit *lineEdit) {
    auto size = data.currentSetSize();                  // Размер вестора 
    auto sideSize = (unsigned int)std::sqrt(size);      // Размер стороны изображения 
    auto img = std::make_unique<QImage>(sideSize, sideSize, QImage::Format_Grayscale16); 

    for(auto i = 0ul; i < size; ++i) {
        auto color = (unsigned int)(data[i] * COLOR_COUNT);
        if(color == COLOR_COUNT) color--;
        img->setPixelColor({int(i % sideSize), int(i / sideSize)}, QColor(color, color, color));
    }

    imgViewer->setImage(std::move(img));
    imgViewer->update(); 

    lineEdit->setText(QString::number(data.getValue()));
}

void DatasetBuilder::extractData(const std::string &fileName) {
    auto greyImg = std::make_shared<Magick::Image>(fileName);
    greyImg->type(Magick::GrayscaleType);

    auto monoImg = std::make_shared<Magick::Image>(*greyImg);
    monoImg->type(Magick::BilevelType);

    Segmenter::Formula formula(
            greyImg, 
            monoImg, 
            Segmenter::Rect{0ul, 0ul, greyImg->columns(), greyImg->rows()}, 
            Segmenter::Formula::Horizontal
        );

    formula.slice();

    auto func = [this](Segmenter::Formula *f) {
        if(f->isLeaf()) {
            this->imageData.add(f->toVector(32));   // Магическое число подумать!!!
        }
    };

    formula.postorder(func);
}
