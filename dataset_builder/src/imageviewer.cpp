/* imageviewer.cpp */

#include "imageviewer.h"

#include <QPainter>

ImageViewer::ImageViewer(QWidget *parent) 
    :QWidget(parent)
{}

void ImageViewer::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    if(img) {
        painter.drawImage(rect(), *img);
    }
}

void ImageViewer::setImage(std::unique_ptr<QImage> img) {
    this->img = std::move(img); 
}
