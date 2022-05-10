#ifndef MAPMANAGER_H
#define MAPMANAGER_H
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDir>
#include <QString>
#include <QMessageLogger>
#include <QDateTime>
#include <QFile>

struct mapElements
{
    inline const static QString MAP_ELEMENT="map";
    inline const static QString MAP_DESCRIPTION="mapDesc";

};
struct mapAttributes
{
    inline const static QString MAP_SUBJECT="mapSubject";
};

class Mapmanager
{


public:
    Mapmanager();

    QString makeNewMap(QString mapName);

    QString getMapSubject(QString mapPath);

private:
    bool establishMapsDir();
    QString generateMapName();


    QString getMapsDirPath();
};

#endif // MAPMANAGER_H
