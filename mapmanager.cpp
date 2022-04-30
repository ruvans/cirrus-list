#include "mapmanager.h"

Mapmanager::Mapmanager()
{

}

bool Mapmanager::makeNewMap(QString mapName)
{
    QString mapsDir = getMapsDir();
    qInfo("Will attempt to create map file at: %s", qUtf8Printable(mapsDir));

    return true;
}

QString Mapmanager::getMapsDir()
{
    QString mapsDir = QDir::current().absoluteFilePath("maps");
    return mapsDir;
}
