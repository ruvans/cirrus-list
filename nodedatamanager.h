#ifndef NODEDATAMANAGER_H
#define NODEDATAMANAGER_H
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QMessageLogger>
/*
 * NodeProperties is all the data required to represent a node
*/
struct NodeProperties
{
    int nodeID;
    int x;
    int y;
    int width;
    int height;
    QString nodeText;
};

/*
 * NodeDataManager is in charge of how node xml is managed
*/
class NodeDataManager
{
public:
    NodeDataManager();
    QString createNodeXml(NodeProperties* nodeProperties);
};

#endif // NODEDATAMANAGER_H
