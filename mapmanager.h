#ifndef MAPMANAGER_H
#define MAPMANAGER_H
#include <QDir>
#include <QString>
#include <QMessageLogger>
#include <QDateTime>
#include <QFile>
#include "mapdatamanager.h"


/* This class is getting maps, sorting maps, etc*/
class Mapmanager
{
public:
    Mapmanager();

    QString makeNewMap(QString mapName);//this will use mapdatamamager to make the file

    QDateTime getLastUpdatedDate(QString mapPath);

    std::vector<MapData> getAvailableMaps();


private:
    bool establishMapsDir();
    QString generateMapName();


    QString getMapsDirPath();
};

#endif // MAPMANAGER_H
