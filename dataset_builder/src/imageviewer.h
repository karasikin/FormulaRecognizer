/* imageviewer.h */

#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <memory>

#include <QWidget>
#include <QImage>

class ImageViewer : public QWidget {
    Q_OBJECT
    
    public:

        ImageViewer(QWidget *parent = nullptr);
        void setImage(std::unique_ptr<QImage> img); 

    protected:

        void paintEvent(QPaintEvent *event) override;

    private:

        std::unique_ptr<QImage> img;


};

#endif // IMAGEVIEWER_H
