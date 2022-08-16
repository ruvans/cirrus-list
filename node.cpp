#include "node.h"

Node::Node(QWidget *parent) : QWidget(parent)
{
    //note: maybe use QPaintEvent for background later on?
    m_backgroundImg = std::make_unique<QPixmap>(":resources/cloudbackground.jpg");
    QPixmap backgroundImg(":resources/cloudbackground.jpg");
    backgroundImg = backgroundImg.scaledToWidth(this->width());
/*
    m_backgroundImgLabel = std::make_unique<QLabel>("", this);
    m_backgroundImgLabel->setFixedSize(this->width(), this->height());
    m_backgroundImgLabel->setPixmap(backgroundImg);
    m_backgroundImgLabel->setMouseTracking(false);

    m_mainText = std::make_unique<QLabel>("new text", m_backgroundImgLabel.get());
    m_mainText->setFixedSize(this->width(), this->height());
    m_mainText->setAlignment(Qt::AlignCenter);
    m_mainText->setStyleSheet("QLabel {  color : black; }");
    m_mainText->setMouseTracking(false);
    */
}

void Node::setText(QString newText)
{
    //m_mainText->setText(newText);
    m_nodeText = newText;
    repaint();
}

void Node::paintEvent(QPaintEvent */*event*/)
{
    QRect nodeRect = this->geometry();
    QFontMetrics metric(font());
    QSize size = metric.size(Qt::TextSingleLine, m_nodeText);

    QImage image(size.width() + 12, size.height() + 12, QImage::Format_ARGB32_Premultiplied);
    image.fill(qRgba(0, 0, 0, 50));

    QFont font;
    font.setStyleStrategy(QFont::ForceOutline);

    QPainter painter(this);
    //paint cloudy background
    QPixmap backgroundImg(":resources/cloudbackground.jpg");
    painter.drawPixmap(0,0,nodeRect.width(),nodeRect.height(), backgroundImg);

    //write node text
    painter.setFont(font);
    painter.setPen(Qt::black);
    painter.drawText(QRect(QPoint(6, 6), size), Qt::AlignCenter, m_nodeText);
    painter.end();


}




