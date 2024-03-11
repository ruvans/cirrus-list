#include "nodedatamanager.h"

NodeDataManager::NodeDataManager()
{

}

void NodeDataManager::addNodeXml(NodeProperties *nodeProperties, QDomDocument& doc)
{
    QDomElement nodeXml = doc.createElement(nodeElements::NODE);
    doc.appendChild(nodeXml);
    nodeXml.setAttribute(nodeAttributes::NODE_ID, nodeProperties->nodeID);
    nodeXml.setAttribute(nodeAttributes::NODE_POSITION_X, nodeProperties->x);
    nodeXml.setAttribute(nodeAttributes::NODE_POSITION_Y, nodeProperties->y);
    nodeXml.setAttribute(nodeAttributes::NODE_WIDTH, nodeProperties->width);
    nodeXml.setAttribute(nodeAttributes::NODE_HEIGHT, nodeProperties->height);
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

    QDomElement tagChildNodes = doc.createElement(nodeElements::CHILD);
    tagNode.appendChild(tagChildNodes);

    return tagNode;
}
