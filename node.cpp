#include "node.h"

Node::Node(QWidget *parent) : QWidget(parent)
{
    //note: maybe use QPaintEvent for background later on?
    m_backgroundImg = std::make_unique<QPixmap>(":resources/cloudbackground.jpg");
    QPixmap backgroundImg(":resources/cloudbackground.jpg");
    backgroundImg = backgroundImg.scaledToWidth(this->width());

    m_backgroundImgLabel = std::make_unique<QLabel>("", this);
    m_backgroundImgLabel->setFixedSize(this->width(), this->height());
    m_backgroundImgLabel->setPixmap(backgroundImg);

    m_mainText = std::make_unique<QLabel>("new text", m_backgroundImgLabel.get());
    m_mainText->setFixedSize(this->width(), this->height());
    m_mainText->setAlignment(Qt::AlignCenter);
    m_mainText->setStyleSheet("QLabel {  color : black; }");
}

void Node::setText(QString newText)
{
    m_mainText->setText(newText);
}
/*
void Node::dragMoveEvent(QDragMoveEvent *event)
{
    qInfo("dragMoveEvent");
}

void Node::dropEvent(QDropEvent *event)
{
    qInfo("dropEvent");
}

void Node::dragEnterEvent(QDragEnterEvent *event)
{
    qInfo("dragEnterEvent");
}

*/
//https://doc.qt.io/qt-5/qtwidgets-draganddrop-draggableicons-example.html
//I think this stuff should be in the parent instead?
//void Node::mouseMoveEvent(QMouseEvent *event)
//{
    /*
    qInfo("drag detected");
    QPoint point = event->pos() - m_lastMousePoint;
    m_backgroundImgLabel->move(point.x(), point.y());

    m_lastMousePoint = event->pos();

*/

//}

//void Node::mousePressEvent(QMouseEvent *event)
//{
    /*
    QPixmap pixmap = m_backgroundImgLabel->pixmap(Qt::ReturnByValue);

     QByteArray itemData;
     QDataStream dataStream(&itemData, QIODevice::WriteOnly);
     dataStream << pixmap << QPoint(event->pos() - m_backgroundImgLabel->pos());
     */
//}

//void Node::mouseReleaseEvent(QMouseEvent *event)
//{

//}


