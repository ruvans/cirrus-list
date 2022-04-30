#ifndef MAPMANAGER_H
#define MAPMANAGER_H
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDir>
#include <QString>
#include <QMessageLogger>
#include <QDateTime>
#include <QFile>

class Mapmanager
{
public:
    Mapmanager();

    bool makeNewMap(QString mapName);
private:
    bool establishMapsDir();
    QString generateMapName();


    QString getMapsDirPath();
};

#endif // MAPMANAGER_H
