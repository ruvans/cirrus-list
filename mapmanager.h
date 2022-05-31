#ifndef MAPMANAGER_H
#define MAPMANAGER_H
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDir>
#include <QString>
#include <QMessageLogger>
#include <QDateTime>
#include <QFile>

struct mapData
{
    QString mapFilename;
    QString mapPath;
    QString mapSubject;
    QString lastUpdated;//probably not a string
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

class Mapmanager
{


public:
    Mapmanager();

    QString makeNewMap(QString mapName);

    QString getMapSubject(QString mapPath);

    std::vector<mapData> getAvailableMaps();



private:
    bool establishMapsDir();
    QString generateMapName();


    QString getMapsDirPath();
};

#endif // MAPMANAGER_H
