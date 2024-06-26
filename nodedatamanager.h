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
    inline const static QString CHILDREN = "children";
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
    int nodeID{-1};
    int x{0};
    int y{0};
    int width{400};
    int height{100};
    QString nodeText{""};
    std::vector<int> children;
};

/*
 * NodeDataManager is in charge of how node xml is managed
*/
class NodeDataManager
{
public:
    NodeDataManager();

    /**
     * @brief addNodeXml Takes a NodeProperties object and creates a node xml tag populated with the
     * nodeProperties data
     * @param nodeProperties[in]
     * @param newNodeElement[out]
     */
    static void addNodeXml(NodeProperties& nodeProperties, QDomElement& newNodeElement);

    /**
     * @brief getInitialNodeData
     * @param nodeText
     * @return QDomElement, an empty node xml tag ready for data
     */
    static QDomElement getInitialNodeData(QString nodeText);

};

#endif // NODEDATAMANAGER_H
