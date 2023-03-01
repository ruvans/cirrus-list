#include "mapdatamanager.h"

MapDataManager::MapDataManager()
{

}

QString MapDataManager::getNewMapData(QString mapSubject)
{
    QString emptyMapData;
    QXmlStreamWriter *xmlWriter = new QXmlStreamWriter(&emptyMapData);
    xmlWriter->writeStartDocument();
    xmlWriter->setAutoFormatting(true);
    xmlWriter->writeStartElement(mapElements::MAP_ELEMENT);
    xmlWriter->writeStartElement(mapElements::MAP_DESCRIPTION);
    xmlWriter->writeTextElement(mapAttributes::MAP_SUBJECT, mapSubject);
    xmlWriter->writeEndElement();
    xmlWriter->writeEndDocument();
    return emptyMapData;
}


QString MapDataManager::getMapSubject(QString mapPath)
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
