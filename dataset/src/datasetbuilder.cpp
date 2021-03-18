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
     data(),
     currentData(-1)
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
    QObject::connect(dataNextBtn, &QPushButton::clicked, this, &DatasetBuilder::onDataNext);
    QObject::connect(dataPrevBtn, &QPushButton::clicked, this, &DatasetBuilder::onDataPrev);
}


void DatasetBuilder::onDataLineEdit() {
    if(currentData >= 0) {
        data[currentData].value = dataLineEdit->text().toInt();
    }
}

void DatasetBuilder::onDataLoad() {
    auto fileName = QFileDialog::getOpenFileName(this, "Open image", ".", "Images (*.png *.jpg)");

    if(fileName.isEmpty()) {
        return;
    }

    data.clear();
    dataNextBtn->setEnabled(false);
    dataPrevBtn->setEnabled(false);

    extractData(fileName.toStdString());

    if(!data.empty()) {
        currentData = 0;
        showData(data[currentData]);
        dataNextBtn->setEnabled(true);
    } else {
        currentData = -1;
    }
    
}

void DatasetBuilder::onDataNext() {
    if(currentData >= 0) {
        dataPrevBtn->setEnabled(true);
    }

    showData(data[++currentData]);

    if(currentData == int(data.size() - 1)) {
        dataNextBtn->setEnabled(false);
    }
}

void DatasetBuilder::onDataPrev() {
    if(currentData <= int(data.size() - 1)) {
        dataNextBtn->setEnabled(true);
    }

    showData(data[--currentData]);

    if(currentData == 0) {
        dataPrevBtn->setEnabled(false);
    }
}

void DatasetBuilder::showData(const Data &data) {
    auto size = data.data->size();                      // Размер вестора 
    auto sideSize = (unsigned int)std::sqrt(size);      // Размер стороны изображения 
    auto img = std::make_unique<QImage>(sideSize, sideSize, QImage::Format_Grayscale16); 

    for(auto i = 0ul; i < size; ++i) {
        auto color = (unsigned int)((*data.data)[i] * COLOR_COUNT);
        if(color == COLOR_COUNT) color--;
        img->setPixelColor({int(i % sideSize), int(i / sideSize)}, QColor(color, color, color));
    }

    dataViewer->setImage(std::move(img));
    dataViewer->update(); 

    dataLineEdit->setText(QString::number(data.value));
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
            this->data.push_back({f->toVector(32), 0});   // Магическое число подумать!!!
        }
    };

    formula.postorder(func);
}
