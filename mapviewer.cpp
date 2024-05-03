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
            qInfo(node->getNodeProperties()->nodeText.toUtf8());
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

    repaint();
}


void MapViewer::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void MapViewer::addChildForSelectedNode()
{
    if (m_grabbedNode == nullptr) {return;}

    int parentID = m_grabbedNode->getNodeProperties()->nodeID;
    NodeProperties newNp = m_currentMap.addNewChildNode(parentID);
    m_nodes.push_back(new Node(this));
    m_nodes.back()->setNodeProperties(newNp);
    m_nodes.back()->setFixedSize(200,40);//todo width and height needs to be dynamic
    m_nodes.back()->show();
    QObject::connect(m_nodes.back(), &Node::nodePropertiesChanged, this, &MapViewer::updataDataForNode);
    //tell the parent so it can draw a connecting line
    m_grabbedNode->addChildID(newNp.nodeID);
    repaint();//draw new line now

    m_grabbedNode->setSelected(false);
    m_nodes.back()->setSelected(true);
    m_nodes.back()->showTextInputBox();
}

void MapViewer::deleteSelectedNode()
{
    if (m_grabbedNode == nullptr) {return;}

    const int ripID = m_grabbedNode->getNodeProperties()->nodeID;
    int nodeIndex(0);

    for(Node* node : m_nodes)
    {
        if (m_grabbedNode == node)
        {
            m_grabbedNode->hide();
            m_nodes.erase(std::next(m_nodes.begin(), nodeIndex));
            break;
        }
        nodeIndex++;
    }

    int parentNodeID=-1;
    m_currentMap.removeNode(ripID, parentNodeID);
    // then find parent node and remove the childID
    if (parentNodeID == -1)
    {
        qInfo("WARNING MapViewer::deleteSelectedNode. cannot find parent node");
        return;
    }
    Node* parent = getNodeObject(parentNodeID);
    if (parent != nullptr){parent->removeChildID(ripID);}
    repaint();

}


void MapViewer::updataDataForNode(Node* node)
{
    m_currentMap.updateNodeData(node->getNodeProperties());
}

void MapViewer::paintEvent(QPaintEvent */*event*/)
{
    drawConnectingLines();
}

void MapViewer::drawConnectingLines()
{
    QPainter painter(this);
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::darkBlue);
    painter.setPen(pen);
    for(auto node : m_nodes)
    {
        //see if node has children
        std::vector<int> kids = node->getNodeProperties()->children;
        if (kids.size() == 0)//no child nodes, no lines needed.
        {
            continue;
        }
        //node has kids, draw lines
        QPoint lineStart, lineEnd;
        lineStart.setX(node->getNodeProperties()->x + 100);
        lineStart.setY(node->getNodeProperties()->y + 20);
        for (auto childID : kids)
        {
            Node* childNode = getNodeObject(childID);
            if (childNode != nullptr)
            {
                lineEnd.setX(childNode->getNodeProperties()->x + 100);//todo width and height needs to be dynamic
                lineEnd.setY(childNode->getNodeProperties()->y + 20); // it will be stored in the node properties later
                painter.drawLine(lineStart.x(), lineStart.y(), lineEnd.x(), lineEnd.y());
            }
        }
    }
}

Node* MapViewer::getNodeObject(int nodeID)
{
    for (auto node : m_nodes)
    {
        if (node->getNodeProperties()->nodeID == nodeID)
        {
            return node;
        }
    }
    return nullptr;
}

//https://stackoverflow.com/questions/18299077/dragging-a-qwidget-in-qt-5
//https://doc.qt.io/qt-5/qtwidgets-draganddrop-fridgemagnets-example.html
