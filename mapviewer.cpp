#include "mapviewer.h"

MapViewer::MapViewer(QString const& mapPath, QWidget *parent) :
    QWidget(parent),
    m_currentMapPath(mapPath)
{
    MapDataManager mapData;

    m_centreNode = std::make_unique<Node>(this);
    m_centreNode->setText(mapData.getMapSubject(mapPath));
    m_centreNode->setFixedSize(200,40);

    setAcceptDrops(true);
}

void MapViewer::saveActiveMap()
{

    NodeDataManager nodeDataManager;
    QString mainNodeXml = nodeDataManager.createNodeXml(m_centreNode->getNodeProperties());

    //tell mapDataManager to create the xml
    //give the xml to the mapManager
    //display some kind of feedback to the user

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
        QPoint hotSpot = event->pos() - m_grabbedNode->pos();
        m_grabbedHotSpot = hotSpot;
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;
        mimeData->setText("application/x-cirrusmap");
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


    QPointF newLocation = event->position() - m_grabbedHotSpot;
    m_grabbedNode->move(newLocation.x() , newLocation.y());
}


void MapViewer::dragEnterEvent(QDragEnterEvent *event)
{
    qInfo("dragEnterEvent");
    event->acceptProposedAction();
}


//https://stackoverflow.com/questions/18299077/dragging-a-qwidget-in-qt-5
//https://doc.qt.io/qt-5/qtwidgets-draganddrop-fridgemagnets-example.html
