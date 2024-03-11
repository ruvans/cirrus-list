#include "mapviewer.h"
#include <iostream>

MapViewer::MapViewer(QString const& mapPath, QWidget *parent) :
    QWidget(parent),
    m_currentMap(mapPath)
{
    std::vector<NodeProperties> nodeProps = m_currentMap.getNodesData();

    std::cout << nodeProps.size() << " nodes" << std::endl;

    for (const NodeProperties& nodeData : nodeProps )
    {
        std::cout << "making node:" << nodeData.nodeText.toStdString() << std::endl;
        m_nodes.push_back(new Node(this));
        m_nodes.back()->setNodeProperties(nodeData);
        m_nodes.back()->setFixedSize(200,40);
    }

    setAcceptDrops(true);
}

void MapViewer::saveActiveMap()
{
    std::vector<NodeProperties*>allNodeProperties;
    for (Node* node : m_nodes)
    {
        allNodeProperties.push_back( node->getNodeProperties());
    }
    //m_currentMap.updateAllNodeData(allNodeProperties);
    Mapmanager mapman;
    mapman.saveMap(m_currentMap);

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
    m_currentMap.updateNodeData(m_grabbedNode->getNodeProperties());
}


void MapViewer::dragEnterEvent(QDragEnterEvent *event)
{
    qInfo("dragEnterEvent");
    event->acceptProposedAction();
}


//https://stackoverflow.com/questions/18299077/dragging-a-qwidget-in-qt-5
//https://doc.qt.io/qt-5/qtwidgets-draganddrop-fridgemagnets-example.html
