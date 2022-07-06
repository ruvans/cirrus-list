#include "mapmanager.h"

Mapmanager::Mapmanager()
{

}

QString Mapmanager::makeNewMap(QString mapName)
{
    if (establishMapsDir() == false)
    {
        return "";
    }

    QString mapSaveName = generateMapName();
    QString mapSavePath = getMapsDirPath().append(QDir::separator()).append(mapSaveName);

    QFile file(mapSavePath);
    file.open(QIODevice::WriteOnly);

    QXmlStreamWriter *xmlWriter = new QXmlStreamWriter(&file);
    xmlWriter->writeStartDocument();
    xmlWriter->setAutoFormatting(true);
    xmlWriter->writeStartElement(mapElements::MAP_ELEMENT);
    xmlWriter->writeStartElement(mapElements::MAP_DESCRIPTION);
    xmlWriter->writeTextElement(mapAttributes::MAP_SUBJECT, mapName);
    xmlWriter->writeEndElement();
    xmlWriter->writeEndDocument();
    file.close();

    return mapSavePath;
}

QString Mapmanager::getMapSubject(QString mapPath)
{
    QString returnStr="";
    QFile file(mapPath);
    file.open(QIODevice::ReadOnly);

    QXmlStreamReader xmlReader(&file);

    while (!xmlReader.atEnd()) {
        if(xmlReader.name() == mapAttributes::MAP_SUBJECT)
        {
            returnStr = xmlReader.readElementText();
            break;
        }
        xmlReader.readNext();

     }
     if (xmlReader.hasError()) {
        //don't panic DONT PANIC
     }

    file.close();
    return returnStr;
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
    QDir mapsDir (getMapsDirPath());
    QStringList mapsList = mapsDir.entryList(QDir::Files,QDir::Time); //find all files, in order of modified time
    for (QString& map : mapsList)
    {
        qInfo("adding map data for map: %s", qUtf8Printable(map));
        MapData newData;
        newData.mapPath = getMapsDirPath().append(QDir::separator()).append(map);
        newData.mapFilename = map;
        newData.mapSubject = getMapSubject(newData.mapPath);
        newData.lastUpdated = getLastUpdatedDate(newData.mapPath);
        mapsData.push_back(newData);
    }

    return mapsData;
}
