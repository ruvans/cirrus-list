#include "mapmanager.h"

Mapmanager::Mapmanager()
{

}

bool Mapmanager::makeNewMap(QString mapName)
{
    if (establishMapsDir() == false)
    {
        return false;
    }

    QString mapSaveName = generateMapName();
    QString mapSavePath = getMapsDirPath().append(QDir::separator()).append(mapSaveName);

    QFile file(mapSavePath);
    file.open(QIODevice::WriteOnly);

    QXmlStreamWriter *xmlWriter = new QXmlStreamWriter(&file);
    xmlWriter->writeStartDocument();
    xmlWriter->setAutoFormatting(true);
    //xmlWriter->writeStartElement("mapDetails");
    xmlWriter->writeTextElement("mapDetails", mapName);
    xmlWriter->writeEndElement();
    xmlWriter->writeEndDocument();
    file.close();

    return true;
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
