#ifndef MAPDATAMANAGER_H
#define MAPDATAMANAGER_H
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QtXml>
#include <QDir>
#include <QString>
#include <QMessageLogger>
#include <QDateTime>
#include <QFile>
#include "node.h"

//Note, in real life applications you will want to save data to another file, ensure save was successful and
//then replace original file with a copy.
struct MapData
{
    QString mapFilename;
    QString mapPath;
    QString mapSubject;
    QDateTime lastUpdated;//probably not a string
    //QString cloudColour;//maybe
};

namespace mapElements
{
    inline const static QString MAP_ELEMENT="map";
    inline const static QString MAP_DESCRIPTION="mapDesc";
    inline const static QString MAP_SUBJECT="mapSubject";
    inline const static QString MAP_NODES="nodes";
};
namespace mapAttributes
{
    inline const static QString MAP_SUBJECT="mapSubject";
    inline const static QString MAP_TOP_ID="topID";
};


/*this class is in charge of how map data files work*/
class MapDataManager
{
public:
    MapDataManager(QString mapPath);

    static QString getNewMapData(QString mapSubject);

    QString getCurrentMapDescription();

    /**
     * @brief getCurrentMapTopID Get the next node ID
     * I rather imagine this to be like one of those ticket dispensors
     * in those old supermarket meat counter where you take a ticket
     * and the number increases for the next person who takes a ticket.
     * @return new node ID
     */
    int getCurrentMapTopID();

    std::vector<NodeProperties>getNodesData();

    void updateNodeData(NodeProperties* nodeProperties);

    void updateAllNodeData(std::vector<NodeProperties*> nodeProperties);

    NodeProperties addNewChildNode(int parentNodeID);

    /**
     * @brief getChildrenIDs Get a list of the children of a given node.
     * This only includes direct children, no grandkiddies included.
     * @param parentNodeID The ID of the parent node
     * @return A list of children node IDs connected to the parent node,
     * may be empty if there are none.
     */
    std::vector<int> getChildrenIDs(int parentNodeID);

    /**
     * @brief getParentID Search for the childNode and return its parents ID
     * @param childNodeID ID of the child node
     * @return ID of the parent node, or -1 if not found.
     */
    int getParentID(int childNodeID);

    /**
     * @brief removeNode deleted a node from the map data
     * @param[in] nodeID the ID of the node to be deleted
     * @param[out] the parentID of the deleted node.
     */
    void removeNode(int nodeID, int& parentID);

    QString m_currentMapData;
    QString m_currentMapPath;

private:
    QDomElement getNodeElementWithID(QDomElement& nodeRoot, int nodeID);
    QDomElement getNodeElementWithID(int nodeID);//same as above but if the QDomElement is only needed for data collecting
    QString getMapAttribute(QString mapAttribute);
    void collectNodesData(std::vector<NodeProperties>& nodes, QDomElement& nodeElement);
    void setTopID();
    bool recursiveNodeSearch(QDomElement& nodeElement, int nodeID, QDomElement& foundNode);
};

#endif // MAPDATAMANAGER_H
