#include "mapmanager.h"

Mapmanager::Mapmanager()
{

}

QString Mapmanager::makeNewMap(QString mapName)
{
    MapDataManager mapDataManager;
    if (establishMapsDir() == false)
    {
        return "";
    }

    QString mapSaveName = generateMapName();
    QString mapSavePath = getMapsDirPath().append(QDir::separator()).append(mapSaveName);
    QString emptyMapData = mapDataManager.getNewMapData(mapName);

    QFile file(mapSavePath);
    file.open(QIODevice::WriteOnly);
    file.write(emptyMapData.toUtf8());
    file.close();

    return mapSavePath;
}


QDateTime Mapmanager::getLastUpdatedDate(QString mapPath)
{
    QFile file(mapPath);
    return file.fileTime(QFileDevice::FileModificationTime);
}

bool Mapmanager::establishMapsDir()
{

    QString mapsDirPath = getMapsDirPath();
    QDir mapsDir(mapsDirPath);
    bool mapsDirExists = mapsDir.exists();
    //Create folder path if needed

    if (mapsDirExists == false)
    {
        mapsDirExists = mapsDir.mkdir(mapsDirPath);
        if (mapsDirExists == false)
        {
            qInfo("Failed to create map file directory at: %s", qUtf8Printable(mapsDirPath));
        }
    }
    return mapsDirExists;
}

QString Mapmanager::generateMapName()
{
    QString mapSaveName = QDateTime::currentDateTime().toString();
    mapSaveName = mapSaveName.replace(QChar(' '), QChar('_'));
    mapSaveName = mapSaveName.replace(QChar(':'), QChar('-'));
    mapSaveName.append("cirruslist.xml");
    return mapSaveName;
}

QString Mapmanager::getMapsDirPath()
{
    QString mapsDir = QDir::current().absoluteFilePath("maps");
    return mapsDir;
}

std::vector<MapData> Mapmanager::getAvailableMaps()
{
    std::vector<MapData> mapsData;
    MapDataManager mapDataManager;
    QDir mapsDir (getMapsDirPath());
    QStringList mapsList = mapsDir.entryList(QDir::Files,QDir::Time); //find all files, in order of modified time
    for (QString& map : mapsList)
    {
        qInfo("adding map data for map: %s", qUtf8Printable(map));
        MapData newData;
        newData.mapPath = getMapsDirPath().append(QDir::separator()).append(map);
        newData.mapFilename = map;
        newData.mapSubject = mapDataManager.getMapSubject(newData.mapPath);
        newData.lastUpdated = getLastUpdatedDate(newData.mapPath);
        mapsData.push_back(newData);
    }

    return mapsData;
}


//todo: finish this and get mapviewer to call him
bool updateExistingMap(QString mapPath)
{
    bool success=false;

    return success;
}
