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

//todo
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
    QDomElement node = root.firstChild().toElement();

    std::vector<NodeProperties> nodes;
    while (node.isNull() == false)
    {
        std::cout << "node isnt null" << std::endl;
        qInfo("node not null");
        if(node.tagName() == nodeElements::NODE)
        {
            std::cout << "node found" << std::endl;
            while(!node.isNull())
            {
                NodeProperties nodeProperties;
                nodeProperties.nodeID = node.attribute(nodeAttributes::NODE_ID).toInt();
                nodeProperties.nodeText = node.attribute(nodeAttributes::NODE_TEXT);
                nodeProperties.x = node.attribute(nodeAttributes::NODE_POSITION_X).toInt();
                nodeProperties.y = node.attribute(nodeAttributes::NODE_POSITION_Y).toInt();
                nodeProperties.height = node.attribute(nodeAttributes::NODE_HEIGHT).toInt();
                nodeProperties.width = node.attribute(nodeAttributes::NODE_WIDTH).toInt();
                nodes.push_back(nodeProperties);
                node = node.nextSibling().toElement();
            }
        }
        node = node.nextSibling().toElement();
    }




    return nodes;
}


void MapDataManager::updateNodeData(NodeProperties* nodeProperties)
{
    QString nodeid = QString::number(nodeProperties->nodeID);
    qInfo("Updating node data for nodeID: " + nodeid.toUtf8());
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

//todo remove after above function fixed and working okay
void MapDataManager::updateAllNodeData(std::vector<NodeProperties *> nodeProperties)
{
//    qInfo("Updating all the node data for saving, which is: ");
//    NodeDataManager nodeMan;
//    QDomDocument doc(m_currentMapData);
//    QDomElement root = doc.documentElement();
//
//    QDomElement tagMap = doc.createElement(mapElements::MAP_NODES);
//    auto nodes = nodeProperties.size();
//    qInfo("Adding data for this many nodes:" );
//    qInfo(QString::number(nodes).toUtf8());
//
//    for (NodeProperties* nodeProperty : nodeProperties)
//    {
//        nodeMan.addNodeXml(nodeProperty, doc);
//    }
//
//    m_currentMapData = doc.toString();
//    qInfo(m_currentMapData.toUtf8());
}


