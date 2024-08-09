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
        m_nodes.back()->setGeometry(nodeData.x, nodeData.y, nodeData.width, nodeData.height);
        //todo move this all and the dupe code in addChild into its own function
        QObject::connect(m_nodes.back(), &Node::nodePropertiesChanged, this, &MapViewer::updataDataForNode);
        QObject::connect(m_nodes.back(), &Node::signalDragInitiation, this, &MapViewer::startNodeDrag);
        QObject::connect(m_nodes.back(), &Node::signalNodeClicked, this, &MapViewer::nodeWasClicked);
        QObject::connect(m_nodes.back(), &Node::nodeWasResized, this, &MapViewer::refreshConnectingLines);
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


void MapViewer::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void MapViewer::dropEvent(QDropEvent *event)
{
    qInfo("dropEvent");
    if (m_selectedNode == nullptr)
    {
        return;
    }

    if (m_selectedNode->cursor().shape() == Qt::ArrowCursor)
    {
        QPointF newLocation = event->position() - m_grabbedHotSpot;
        m_selectedNode->move(newLocation.x() , newLocation.y());
        m_selectedNode->resetMoveCheckers();
        repaint();
    }

    m_currentMap.updateNodeData(m_selectedNode->getNodeProperties());
}


void MapViewer::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void MapViewer::addChildForSelectedNode()
{
    if (m_selectedNode == nullptr) {return;}

    int parentID = m_selectedNode->getNodeProperties()->nodeID;
    NodeProperties newNp = m_currentMap.addNewChildNode(parentID);
    QPoint childStarterPos = getBestStartingPositionForChild(m_selectedNode);
    newNp.x = childStarterPos.x();
    newNp.y = childStarterPos.y();
    m_nodes.push_back(new Node(this));
    m_nodes.back()->setNodeProperties(newNp);
    m_nodes.back()->show();
    QObject::connect(m_nodes.back(), &Node::nodePropertiesChanged, this, &MapViewer::updataDataForNode);
    QObject::connect(m_nodes.back(), &Node::signalDragInitiation, this, &MapViewer::startNodeDrag);
    QObject::connect(m_nodes.back(), &Node::signalNodeClicked, this, &MapViewer::nodeWasClicked);
    QObject::connect(m_nodes.back(), &Node::nodeWasResized, this, &MapViewer::refreshConnectingLines);
    //tell the parent so it can draw a connecting line
    m_selectedNode->addChildID(newNp.nodeID);
    repaint();//draw new line now

    m_selectedNode->setSelected(false);
    m_nodes.back()->setSelected(true);
    m_nodes.back()->showTextInputBox();
}

void MapViewer::deleteSelectedNode()
{
    if (m_selectedNode == nullptr) {return;}

    const int ripID = m_selectedNode->getNodeProperties()->nodeID;
    //collateral damage
    std::vector<int>childNodeIDs = m_currentMap.getChildrenIDsRecursive(ripID);
    //get rid of the dead nodes
    for(int i = 0; i < (int)m_nodes.size(); i++)
    {
        if (m_selectedNode == m_nodes.at(i))
        {
            m_selectedNode->hide();
            m_nodes.erase(std::next(m_nodes.begin(), i));
            break;
        }
    }
    for(auto childID : childNodeIDs)
    {
        for(int i = 0; i < (int)m_nodes.size(); i++)
        {
            if (m_nodes.at(i)->getNodeProperties()->nodeID == childID)
            {
                m_nodes.at(i)->hide();
                m_nodes.erase(std::next(m_nodes.begin(), i));
                continue;
            }
        }
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

void MapViewer::refreshConnectingLines()
{
    update();
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
    painter.setRenderHint( QPainter::Antialiasing );
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
        lineStart.setX(node->x() + (node->width()/2));
        lineStart.setY(node->y() + (node->height()/2));
        for (auto childID : kids)
        {
            Node* childNode = getNodeObject(childID);
            if (childNode != nullptr)
            {
                lineEnd.setX(childNode->x() + (childNode->width()/2));
                lineEnd.setY(childNode->y() + (childNode->height()/2));
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


void MapViewer::startNodeDrag(Node* draggedNode, QPoint mousePos)
{
    m_selectedNode = draggedNode;
    m_grabbedHotSpot = mousePos;
    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;
    mimeData->setText("application/x-cirrusmap");
    QPixmap tempImg = draggedNode->grab();
    drag->setMimeData(mimeData);
    drag->setPixmap(tempImg);
    drag->setHotSpot(mousePos);
    drag->exec();
}

void MapViewer::nodeWasClicked(Node* nodeClicked)
{
    for (Node* node : m_nodes)
    {
        bool nodeNeedsUnselecting = node != nodeClicked && node->isSelected == true;
        if (nodeNeedsUnselecting) node->setSelected(false);
    }
    m_selectedNode = nodeClicked;
    emit nodeSelectionChanged(/*active*/true);
}

void MapViewer::unselectAllNodes()
{
    for (Node* node : m_nodes)
    {
        node->setSelected(false);
    }
    emit nodeSelectionChanged(/*active*/false);
}

void MapViewer::mousePressEvent(QMouseEvent */*event*/)
{
    unselectAllNodes();
}


QPoint MapViewer::getBestStartingPositionForChild(Node* parentNode)
{
    NodeProperties* nodeProps = parentNode->getNodeProperties();
    QPoint pos;
    pos.setX(nodeProps->x + 10);
    pos.setY(nodeProps->y + 10);
    return pos;
}
//https://stackoverflow.com/questions/18299077/dragging-a-qwidget-in-qt-5
//https://doc.qt.io/qt-5/qtwidgets-draganddrop-fridgemagnets-example.html
