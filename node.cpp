#include "node.h"

Node::Node(QWidget *parent) : QWidget(parent)
{
}

void Node::setSelected(bool newState)
{
    if (newState != isSelected)
    {
        isSelected = newState;
        repaint();
    }
}

void Node::setText(QString newText)
{
    m_nodeProperties.nodeText = newText;
    repaint();
}

void Node::setNodeProperties(NodeProperties properties)
{
    m_nodeProperties = properties;
    move(properties.x, properties.y);
}

NodeProperties* Node::getNodeProperties()
{
    return &m_nodeProperties;
}

void Node::paintEvent(QPaintEvent* /*event*/)
{
    const QRect nodeRect(0,0,this->width(), this->height());

    QPainter painter(this);
    //paint cloudy background
    if (isSelected)
    {
        QPixmap backgroundImg(":resources/activecloudbackground.png");
        painter.drawPixmap(0,0,nodeRect.width(),nodeRect.height(), backgroundImg);
    }
    else
    {
        QPixmap backgroundImg(":resources/cloudbackground.png");
        painter.drawPixmap(0,0,nodeRect.width(),nodeRect.height(), backgroundImg);

    }

    //write node text
    QFont font;
    font.setStyleStrategy(QFont::ForceOutline);
    painter.setFont(font);
    painter.setPen(Qt::black);
    painter.drawText(nodeRect, Qt::AlignCenter, m_nodeProperties.nodeText);
    painter.end();
}

void Node::moveEvent(QMoveEvent */*event*/)
{
    //note: this is also triggered on widget initialisation
    qInfo("widget moved");
    m_nodeProperties.x = this->x();
    m_nodeProperties.y = this->y();
}




