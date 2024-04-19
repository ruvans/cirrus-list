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
    tagMapDesc.setAttribute(mapAttributes::MAP_TOP_ID, "0");
    tagMap.appendChild(tagMapDesc);

    QDomElement tagNode = NodeDataManager::getInitialNodeData(mapSubject);
    tagMap.appendChild(tagNode);

    return doc.toString();
}

QString MapDataManager::getCurrentMapDescription()
{
    QString mapDescription = getMapAttribute(mapAttributes::MAP_SUBJECT);
    return mapDescription.isEmpty() ?  "Unknown" : mapDescription;
}

QString MapDataManager::getMapAttribute(QString mapAttribute)
{
    QString result;
    QDomDocument doc;
    doc.setContent(m_currentMapData);
    QDomElement mapRoot = doc.documentElement();
    QDomElement mapDesc = mapRoot.firstChildElement(mapElements::MAP_DESCRIPTION);
    if (mapDesc.hasAttribute(mapAttribute))
    {
        result = mapDesc.attribute(mapAttribute);
    }

    return result;
}

int MapDataManager::getCurrentMapTopID()
{
    QDomDocument doc;
    doc.setContent(m_currentMapData);
    QDomElement mapRoot = doc.documentElement();
    QDomElement mapDesc = mapRoot.firstChildElement(mapElements::MAP_DESCRIPTION);
    int newID = mapDesc.attribute(mapAttributes::MAP_TOP_ID).toInt();
    newID++;
    mapDesc.setAttribute(mapAttributes::MAP_TOP_ID, newID);
    m_currentMapData = doc.toString();
    return newID;
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

bool MapDataManager::recursiveNodeSearch(QDomElement& nodeElement, int nodeID, QDomElement& foundNode)
{
    bool foundYa(false);
    while(foundYa == false || nodeElement.isNull() == false)
    {
        const bool nodeFound = nodeElement.hasAttribute(nodeAttributes::NODE_ID) &&
                               nodeElement.attribute(nodeAttributes::NODE_ID) == QString::number(nodeID);
        if (nodeFound)
        {
            foundNode = nodeElement;
            foundYa = true;
            break;
        }

        QDomElement firstChild = nodeElement.firstChildElement(nodeElements::CHILDREN);
        const bool nodeHasChildren = firstChild.isNull() == false;
        if (nodeHasChildren)
        {
            QDomElement firstChildNode = firstChild.firstChildElement(nodeElements::NODE);
            foundYa = recursiveNodeSearch(firstChildNode, nodeID, foundNode);
        }
        nodeElement = nodeElement.nextSiblingElement(nodeElements::NODE);
    }
    return foundYa;
}

QDomElement MapDataManager::getNodeElementWithID(QDomElement& root, int nodeID)
{
    QDomElement node;
    QDomElement firstNode = root.firstChildElement(nodeElements::NODE);
    bool nodeExisted = recursiveNodeSearch(firstNode, nodeID, node);
    if (nodeExisted == false)
    {
        qInfo("WARNING MapDataManager::getNodeElementWithID did not find the node. That shouldn't happen");
    }
    return node;
}

void MapDataManager::updateNodeData(NodeProperties* nodeProperties)
{
    QDomDocument doc;
    doc.setContent(m_currentMapData);
    QDomElement root = doc.documentElement();

    QDomElement nodeToUpdate = getNodeElementWithID(root, nodeProperties->nodeID);

    if (nodeToUpdate.hasAttribute(nodeAttributes::NODE_ID))//just checking its not empty!
    {
        nodeToUpdate.setAttribute(nodeAttributes::NODE_POSITION_X, nodeProperties->x);
        nodeToUpdate.setAttribute(nodeAttributes::NODE_POSITION_Y, nodeProperties->y);
        nodeToUpdate.setAttribute(nodeAttributes::NODE_TEXT, nodeProperties->nodeText);
        nodeToUpdate.setAttribute(nodeAttributes::NODE_HEIGHT, nodeProperties->height);
        nodeToUpdate.setAttribute(nodeAttributes::NODE_WIDTH, nodeProperties->width);
    }
  m_currentMapData = doc.toString();
}

NodeProperties MapDataManager::addNewChildNode(int parentNodeID)
{
    NodeProperties newNodeProperties;
    newNodeProperties.nodeID = getCurrentMapTopID();
    qInfo("Adding child node");
    qInfo(qUtf8Printable(m_currentMapData));
    QDomDocument doc;
    doc.setContent(m_currentMapData);

    QDomElement root = doc.documentElement();
    QDomElement parentNode = getNodeElementWithID(root, parentNodeID);
    if (parentNode.isNull() == false)
    {
        //if no children node, add one
        QDomElement childrenNode = parentNode.firstChildElement(nodeElements::CHILDREN);
        if (childrenNode.isNull())//no children tag? we'll make one!
        {
            QDomElement newChildrenNode = doc.createElement(nodeElements::CHILDREN);
            parentNode.appendChild(newChildrenNode);
            childrenNode = newChildrenNode;
        }
        QDomElement childNode = doc.createElement(nodeElements::NODE);
        NodeDataManager::addNodeXml(newNodeProperties, childNode);
        childrenNode.appendChild(childNode);
        m_currentMapData = doc.toString();
    }

  qInfo(qUtf8Printable(m_currentMapData));
  return newNodeProperties;
}
