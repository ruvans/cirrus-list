#ifndef MAPDATAMANAGER_H
#define MAPDATAMANAGER_H
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDir>
#include <QString>
#include <QMessageLogger>
#include <QDateTime>
#include <QFile>


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

};
namespace mapAttributes
{
    inline const static QString MAP_SUBJECT="mapSubject";
};

/*this class is in charge of how map data files work*/
class MapDataManager
{
public:
    MapDataManager();

    QString getNewMapData(QString mapSubject);

    QString getMapSubject(QString mapPath);//could be a static func
private:

};

#endif // MAPDATAMANAGER_H
