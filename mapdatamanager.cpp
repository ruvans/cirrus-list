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
         newNodeData.children = getChildrenIDs(newNodeData.nodeID);
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
        if (nodeElement.hasAttribute(nodeAttributes::NODE_ID) == false)
        {
            qInfo("WARNING: MapDataManager::recursiveNodeSearch. Found a node without an ID! that's weird innit");
            break;
        }
        const bool nodeFound =  nodeElement.attribute(nodeAttributes::NODE_ID) == QString::number(nodeID);
        qInfo("Comparing: " + nodeElement.attribute(nodeAttributes::NODE_ID).toUtf8() + " with nodeID " + QString::number(nodeID).toUtf8());
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

QDomElement MapDataManager::getNodeElementWithID(int nodeID)
{
    QDomElement node;
    QDomDocument doc;
    doc.setContent(m_currentMapData);
    QDomElement root = doc.documentElement();
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


std::vector<int> MapDataManager::getChildrenIDs(int parentNodeID)
{
    QDomElement parentNode = getNodeElementWithID(parentNodeID);
    std::vector<int>childIds;
    QDomElement childrenNodes = parentNode.firstChildElement(nodeElements::CHILDREN);
    QDomElement childNode = childrenNodes.firstChildElement(nodeElements::NODE);
    while(childNode.isNull() == false)
    {
        qInfo(childNode.attribute(nodeAttributes::NODE_ID).toUtf8());
        childIds.push_back(childNode.attribute(nodeAttributes::NODE_ID).toInt());
        childNode = childNode.nextSiblingElement(nodeElements::NODE);
    }

    return childIds;
}

std::vector<int> MapDataManager::getChildrenIDsRecursive(int parentNodeID)
{
    std::vector<int>childIDs;
    QDomElement parentNode = getNodeElementWithID(parentNodeID);
    QDomElement childrenNodes = parentNode.firstChildElement(nodeElements::CHILDREN);
    QDomElement childNode = childrenNodes.firstChildElement(nodeElements::NODE);
    if (childNode.isNull() == false)
    {
        getChildrenIDsRecursiveCollectIDs(parentNode, childIDs);
    }
    return childIDs;
}

void MapDataManager::getChildrenIDsRecursiveCollectIDs(QDomElement& parentNode, std::vector<int>& childIDs)
{
    QDomElement childrenNodes = parentNode.firstChildElement(nodeElements::CHILDREN);
    QDomElement childNode = childrenNodes.firstChildElement(nodeElements::NODE);
    while(childNode.isNull() == false)
    {
        childIDs.push_back(childNode.attribute(nodeAttributes::NODE_ID).toInt());
        QDomElement firstGrandchild = childNode.firstChildElement(nodeElements::CHILDREN);
        const bool childNodeHasChildren = firstGrandchild.isNull() == false;
        if (childNodeHasChildren)
        {
          getChildrenIDsRecursiveCollectIDs(childNode, childIDs);
        }
        childNode = childNode.nextSiblingElement(nodeElements::NODE);
    }
}

int MapDataManager::getParentID(int childNodeID)
{
    int ID(-1);
    QDomElement node;
    QDomDocument doc;
    doc.setContent(m_currentMapData);
    QDomElement root = doc.documentElement();
    QDomElement firstNode = root.firstChildElement(nodeElements::NODE);
    bool nodeExisted = recursiveNodeSearch(firstNode, childNodeID, node);
    node = node.parentNode().parentNode().toElement();
    if (nodeExisted == true && node.hasAttribute(nodeAttributes::NODE_ID))
    {
        ID = node.attribute(nodeAttributes::NODE_ID).toInt();
    }
    return ID;
}


void MapDataManager::removeNode(int nodeID, int& parentID)
{
    QDomElement node;
    QDomElement parentNode, childrenNode;
    QDomDocument doc;
    doc.setContent(m_currentMapData);
    QDomElement root = doc.documentElement();
    QDomElement firstNode = root.firstChildElement(nodeElements::NODE);
    bool nodeExisted = recursiveNodeSearch(firstNode, nodeID, node);

    if (nodeExisted == true)
    {
        childrenNode = node.parentNode().toElement();
        parentNode = node.parentNode().parentNode().toElement();
        if (parentNode.hasAttribute(nodeAttributes::NODE_ID))
        {
            parentID = parentNode.attribute(nodeAttributes::NODE_ID).toInt();
            QDomNode updatedNode = childrenNode.removeChild(node);
            if (updatedNode.isNull())
            {
                qInfo("WARNING: MapDataManager::removeNode. Failed to delete node");
            }
        }
    }
    m_currentMapData = doc.toString();
}
