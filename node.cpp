#include "node.h"

node::node(QWidget *parent) : QWidget(parent)
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

void node::setText(QString newText)
{
    m_mainText->setText(newText);
}


