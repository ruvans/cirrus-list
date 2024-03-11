#ifndef NODEDATAMANAGER_H
#define NODEDATAMANAGER_H
#include <QXmlStreamWriter>
#include <QtXml>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QMessageLogger>
//https://thecodeprogram.com/how-to-read-and-write-xml-files-in-qt-c--
namespace nodeElements
{
    inline const static QString NODE = "node";
    inline const static QString CHILD = "children";
    inline const static QString CHILD_NODE = "childnode";
};
namespace nodeAttributes
{
    inline const static QString NODE_ID = "id";
    inline const static QString NODE_TEXT = "text";
    inline const static QString NODE_POSITION_X = "posx";
    inline const static QString NODE_POSITION_Y = "posy";
    inline const static QString NODE_WIDTH = "width";
    inline const static QString NODE_HEIGHT = "height";
};
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
    static void addNodeXml(NodeProperties* nodeProperties, QDomDocument& parentDoc);

    static QDomElement getInitialNodeData(QString nodeText);
};

#endif // NODEDATAMANAGER_H
