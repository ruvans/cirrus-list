#include "mapviewer.h"
#include <iostream>

MapViewer::MapViewer(QString const& mapPath, QWidget *parent) :
    QWidget(parent),
    m_currentMap(mapPath)
{
    this->setObjectName("mapArea");
    std::vector<NodeProperties> nodeProps = m_currentMap.getNodesData();

    std::cout << nodeProps.size() << " nodes" << std::endl;

    for (const NodeProperties& nodeData : nodeProps )
    {
        std::cout << "making node:" << nodeData.nodeText.toStdString() << std::endl;
        m_nodes.push_back(new Node(this));
        m_nodes.back()->setNodeProperties(nodeData);
        m_nodes.back()->setFixedSize(200,40);
        QObject::connect(m_nodes.back(), &Node::nodePropertiesChanged, this, &MapViewer::updataDataForNode);
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
    Mapmanager mapman;
    mapman.saveMap(m_currentMap);

}


void MapViewer::mousePressEvent(QMouseEvent *event)
{
    qInfo("mousePressEvent");
    m_grabbedNode = static_cast<Node*>(childAt(event->pos()));
    bool nodeWasGrabbed = m_grabbedNode != nullptr;

    if (nodeWasGrabbed && event->button() == Qt::LeftButton)
    {
        //This will make sure only the grabbed node is selected
        //that way we can be certain whose child the new node will belong to
        for(auto node: m_nodes)
        {
            bool selectedNode = node == m_grabbedNode;
            node->setSelected(selectedNode);
        }
        //m_grabbedNode->setSelected(true);
        emit nodeSelectionChanged(/*active*/true);

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

    if (!nodeWasGrabbed)
    {
        for (Node* node : m_nodes)
        {
            node->setSelected(false);
        }
        emit nodeSelectionChanged(/*active*/false);
    }

}

void MapViewer::mouseReleaseEvent(QMouseEvent */*event*/)
{
    //see dropEvent
}

void MapViewer::mouseMoveEvent(QMouseEvent */*event*/)
{
}

void MapViewer::dragMoveEvent(QDragMoveEvent *event)
{
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
    event->acceptProposedAction();
}

void MapViewer::addChildForSelectedNode()
{
    qInfo("mapViewer::addChildForSelectedNode");
    if (m_grabbedNode == nullptr)
    {
        return;
    }
    int parentID = m_grabbedNode->getNodeProperties()->nodeID;
    NodeProperties newNp = m_currentMap.addNewChildNode(parentID);
    m_nodes.push_back(new Node(this));
    m_nodes.back()->setNodeProperties(newNp);
    m_nodes.back()->setText("new node");
    m_nodes.back()->setFixedSize(200,40);
    m_nodes.back()->show();
    QObject::connect(m_nodes.back(), &Node::nodePropertiesChanged, this, &MapViewer::updataDataForNode);

    m_grabbedNode->setSelected(false);
    m_nodes.back()->setSelected(true);
    m_nodes.back()->showTextInputBox();
}

void MapViewer::deleteSelectedNode()
{

}


void MapViewer::updataDataForNode(Node* node)
{
    m_currentMap.updateNodeData(node->getNodeProperties());
}
//https://stackoverflow.com/questions/18299077/dragging-a-qwidget-in-qt-5
//https://doc.qt.io/qt-5/qtwidgets-draganddrop-fridgemagnets-example.html
