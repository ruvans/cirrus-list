#ifndef MAPMANAGER_H
#define MAPMANAGER_H
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDir>
#include <QString>
#include <QMessageLogger>

class Mapmanager
{
public:
    Mapmanager();

    bool makeNewMap(QString mapName);
private:
    QString getMapsDir();
};

#endif // MAPMANAGER_H
