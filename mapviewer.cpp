#include "mapviewer.h"

MapViewer::MapViewer(QString const& mapPath, QWidget *parent) : QWidget(parent)
{
    Mapmanager mapman;

    m_centreNode = std::make_unique<Node>(this);
    m_centreNode->setText(mapman.getMapSubject(mapPath));
    m_centreNode->setFixedSize(200,40);
    setAcceptDrops(true);
}


void MapViewer::mousePressEvent(QMouseEvent *event)
{
    qInfo("mousePressEvent");
    m_grabbedNode = static_cast<Node*>(childAt(event->pos()));
    if (m_grabbedNode == nullptr)
    {
        return;
    }

    if (event->button() == Qt::LeftButton)
    {
        qInfo("event pos: %d.%d", event->pos().x(), event->pos().y());
        qInfo("child pos: %d.%d", m_grabbedNode->pos().x(), m_grabbedNode->pos().y());
        QPoint hotSpot = event->pos() - m_grabbedNode->pos();
        m_grabbedHotSpot = hotSpot;
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;
        mimeData->setText("application/x-cirrusmap");
        //QPixmap tempImg(":resources/cloudbackground.jpg");
        QPixmap tempImg = m_grabbedNode->grab();
        drag->setMimeData(mimeData);
        drag->setPixmap(tempImg);
        drag->setHotSpot(hotSpot);
        drag->exec();


    }

}

void MapViewer::mouseReleaseEvent(QMouseEvent *event)
{
    qInfo("mouse released");
}

void MapViewer::mouseMoveEvent(QMouseEvent *event)
{
    qInfo("mouseMoveEvent");
}

void MapViewer::dragMoveEvent(QDragMoveEvent *event)
{
    //qInfo("dragMoveEvent");
    event->acceptProposedAction();
}

void MapViewer::dropEvent(QDropEvent *event)
{
    qInfo("dropEvent");
    if (m_grabbedNode == nullptr)
    {
        return;
    }


    QPointF newLocation = event->position();
    m_grabbedNode->move(newLocation.x() - m_grabbedHotSpot.x() , newLocation.y() - m_grabbedHotSpot.y());

}


void MapViewer::dragEnterEvent(QDragEnterEvent *event)
{
    qInfo("dragEnterEvent");
    event->acceptProposedAction();
}


//https://stackoverflow.com/questions/18299077/dragging-a-qwidget-in-qt-5
//https://doc.qt.io/qt-5/qtwidgets-draganddrop-fridgemagnets-example.html
