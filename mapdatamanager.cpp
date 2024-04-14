#include "mapdatamanager.h"
#include <iostream>
MapDataManager::MapDataManager(QString mapPath)
{
    QString mapString;
    QFile file(mapPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        mapString.append(line);
    }
    file.close();
    m_currentMapData = mapString;
    m_currentMapPath = mapPath;
}

QString MapDataManager::getNewMapData(QString mapSubject)
{
    QDomDocument doc;
    QDomElement tagMap = doc.createElement(mapElements::MAP_ELEMENT);
    doc.appendChild(tagMap);

    QDomElement tagMapDesc = doc.createElement(mapElements::MAP_DESCRIPTION);
    tagMapDesc.setAttribute(mapAttributes::MAP_SUBJECT, mapSubject);
    tagMap.appendChild(tagMapDesc);


    QDomElement tagNode = NodeDataManager::getInitialNodeData(mapSubject);
    tagMap.appendChild(tagNode);

    return doc.toString();
}

QString MapDataManager::getCurrentMapDescription()
{
    QString mapDescription("Unknown");

    QDomDocument doc;
    doc.setContent(m_currentMapData);
    QDomElement mapRoot = doc.documentElement();
    QDomElement mapDesc = mapRoot.firstChildElement(mapElements::MAP_DESCRIPTION);
    if (mapDesc.hasAttribute(mapAttributes::MAP_SUBJECT))
    {
        mapDescription = mapDesc.attribute(mapAttributes::MAP_SUBJECT);
    }

    return mapDescription;
}

std::vector<NodeProperties> MapDataManager::getNodesData()
{
    qInfo("getNodesData called");
    qInfo(m_currentMapData.toUtf8());
    QDomDocument mapXml;

     QFile xmlFile(m_currentMapPath);
     xmlFile.open(QIODevice::ReadOnly);
     mapXml.setContent(&xmlFile);
     xmlFile.close();

    QDomElement root = mapXml.documentElement();
    QDomElement node = root.firstChildElement(nodeElements::NODE);

    std::vector<NodeProperties> nodeProperties;
    collectNodesData(nodeProperties, node);
    return nodeProperties;
}

void MapDataManager::collectNodesData(std::vector<NodeProperties>& nodeProperties, QDomElement& nodeElement)
{
    for (; nodeElement.isNull() == false; nodeElement = nodeElement.nextSiblingElement(nodeElements::NODE))
    {
         //todo nodeDataManager should do this stuff
         NodeProperties newNodeData;
         newNodeData.nodeID = nodeElement.attribute(nodeAttributes::NODE_ID).toInt();
         newNodeData.nodeText = nodeElement.attribute(nodeAttributes::NODE_TEXT);
         newNodeData.x = nodeElement.attribute(nodeAttributes::NODE_POSITION_X).toInt();
         newNodeData.y = nodeElement.attribute(nodeAttributes::NODE_POSITION_Y).toInt();
         newNodeData.height = nodeElement.attribute(nodeAttributes::NODE_HEIGHT).toInt();
         newNodeData.width = nodeElement.attribute(nodeAttributes::NODE_WIDTH).toInt();
         nodeProperties.push_back(newNodeData);
         QDomElement firstChild = nodeElement.firstChildElement(nodeElements::CHILDREN);
         const bool nodeHasChildren = firstChild.isNull() == false;
         if (nodeHasChildren)
         {
             QDomElement firstChildNode = firstChild.firstChildElement(nodeElements::NODE);
             collectNodesData(nodeProperties, firstChildNode);
         }
    }
}

void MapDataManager::updateNodeData(NodeProperties* nodeProperties)
{
    QDomDocument doc;
    doc.setContent(m_currentMapData);
    QDomElement root = doc.documentElement();

    QDomElement node = root.firstChild().toElement();

    while (node.isNull() == false)
    {
        const bool nodeFound = node.hasAttribute(nodeAttributes::NODE_ID) &&
                               node.attribute(nodeAttributes::NODE_ID) == QString::number(nodeProperties->nodeID);
        if( nodeFound ==  false)
        {
            node = node.nextSibling().toElement();
            continue;
        }
         node.setAttribute(nodeAttributes::NODE_POSITION_X, nodeProperties->x);
         node.setAttribute(nodeAttributes::NODE_POSITION_Y, nodeProperties->y);
         node.setAttribute(nodeAttributes::NODE_TEXT, nodeProperties->nodeText);
         node.setAttribute(nodeAttributes::NODE_HEIGHT, nodeProperties->height);
         node.setAttribute(nodeAttributes::NODE_WIDTH, nodeProperties->width);
         break;
    }
  m_currentMapData = doc.toString();
}

void MapDataManager::addNewChildNode(int parentNodeID)
{
    NodeProperties newNodeProperties;
    qInfo("Adding child node");
    QDomDocument doc;
    doc.setContent(m_currentMapData);
    QDomElement root = doc.documentElement();

    QDomElement node = root.firstChild().toElement();

    while (node.isNull() == false)
    {
        const bool nodeFound = node.hasAttribute(nodeAttributes::NODE_ID) &&
                node.attribute(nodeAttributes::NODE_ID) == QString::number(parentNodeID);
        if( nodeFound ==  false)
        {
            node = node.nextSibling().toElement();
            continue;
        }
        qInfo("parent node was found. Adding child node data now");
        //add child node here
        QDomElement childrenNode = node.firstChild().toElement();

        QDomElement childNode = doc.createElement(nodeElements::NODE);
        NodeDataManager::addNodeXml(newNodeProperties, childNode);

        childrenNode.appendChild(childNode);
        break;
    }
  m_currentMapData = doc.toString();
  qInfo(qUtf8Printable(m_currentMapData));

}
