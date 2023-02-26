#include "mapdatamanager.h"

MapDataManager::MapDataManager()
{

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
