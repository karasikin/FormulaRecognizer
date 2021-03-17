/* datasetbuilder.cpp */

#include "datasetbuilder.h"

#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>

#include "segmenter/formula.h"

#include <QDebug>


DatasetBuilder::DatasetBuilder(QWidget *parent) 
    :QWidget(parent)
{
    datasetImg = new QLabel;
    //databaseLabel.setMinimumSize({
    datasetLineEdit = new QLineEdit;
    datasetLoadBtn = new QPushButton("Load");
    datasetDeleteBtn = new QPushButton("Delete");
    datasetPrevBtn = new QPushButton("<");
    datasetNextBtn = new QPushButton(">");

    dataImg = new QLabel;
    dataLineEdit = new QLineEdit;
    dataLoadBtn = new QPushButton("Load");
    dataAddBtn = new QPushButton("Add");
    dataPrevBtn = new QPushButton("<");
    dataNextBtn = new QPushButton(">");

    auto datasetNavigationLayout = new QHBoxLayout;
    datasetNavigationLayout->addWidget(datasetPrevBtn);
    datasetNavigationLayout->addWidget(datasetNextBtn);

    auto datasetActionLayout = new QVBoxLayout;
    datasetActionLayout->addWidget(datasetLineEdit);
    datasetActionLayout->addWidget(datasetLoadBtn);
    datasetActionLayout->addWidget(datasetDeleteBtn);

    auto datasetTopLayout = new QHBoxLayout;
    datasetTopLayout->addWidget(datasetImg);
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
    dataTopLayout->addWidget(dataImg);
    dataTopLayout->addLayout(dataActionLayout);

    auto dataLayout = new QVBoxLayout;
    dataLayout->addLayout(dataTopLayout);
    dataLayout->addLayout(dataNavigationLayout);

    auto *layout = new QHBoxLayout;
    layout->addLayout(dataLayout);
    layout->addLayout(datasetLayout);

    setLayout(layout);

    QObject::connect(dataLoadBtn, &QPushButton::clicked, this, &DatasetBuilder::onDataLoad);
}

void DatasetBuilder::onDataLoad() {
    auto fileName = QFileDialog::getOpenFileName(this, "Open image", ".", "Images (*.png *.jpg)");

    if(fileName.isEmpty()) {
        return;
    }
    
    auto greyImg = std::make_shared<Magick::Image>(fileName.toStdString());
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
            this->data.push_back({f->toVector(32), 0});
        }
    };

    formula.postorder(func);
}


