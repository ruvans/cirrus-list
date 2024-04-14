#include "nodedatamanager.h"

NodeDataManager::NodeDataManager()
{

}

void NodeDataManager::addNodeXml(NodeProperties& nodeProperties, QDomElement& newNodeElement)
{
    newNodeElement.setAttribute(nodeAttributes::NODE_ID, nodeProperties.nodeID);
    newNodeElement.setAttribute(nodeAttributes::NODE_POSITION_X, nodeProperties.x);
    newNodeElement.setAttribute(nodeAttributes::NODE_POSITION_Y, nodeProperties.y);
    newNodeElement.setAttribute(nodeAttributes::NODE_WIDTH, nodeProperties.width);
    newNodeElement.setAttribute(nodeAttributes::NODE_HEIGHT, nodeProperties.height);
}

QDomElement NodeDataManager::getInitialNodeData(QString nodeText)
{
    QDomDocument doc;
    QDomElement tagNode = doc.createElement(nodeElements::NODE);
    doc.appendChild(tagNode);
    tagNode.setAttribute(nodeAttributes::NODE_ID, 0);
    tagNode.setAttribute(nodeAttributes::NODE_POSITION_X, 0);
    tagNode.setAttribute(nodeAttributes::NODE_POSITION_Y, 0);
    tagNode.setAttribute(nodeAttributes::NODE_WIDTH, 0);
    tagNode.setAttribute(nodeAttributes::NODE_HEIGHT, 0);
    tagNode.setAttribute(nodeAttributes::NODE_TEXT, nodeText);

    QDomElement tagChildNodes = doc.createElement(nodeElements::CHILDREN);
    tagNode.appendChild(tagChildNodes);

    return tagNode;
}

