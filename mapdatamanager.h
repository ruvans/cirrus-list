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
};


/*this class is in charge of how map data files work*/
class MapDataManager
{
public:
    MapDataManager(QString mapPath);

    static QString getNewMapData(QString mapSubject);

    std::vector<NodeProperties>getNodesData();


    void updateNodeData(NodeProperties* nodeProperties);

    void updateAllNodeData(std::vector<NodeProperties*> nodeProperties);

    QString m_currentMapData;
    QString m_currentMapPath;
};

#endif // MAPDATAMANAGER_H
