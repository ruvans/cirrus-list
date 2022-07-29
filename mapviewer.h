#ifndef MAPVIEWER_H
#define MAPVIEWER_H

#include <QWidget>
#include <QLabel>


#include "mapmanager.h"
#include "node.h"

class MapViewer : public QWidget
{
    Q_OBJECT
public:
    explicit MapViewer(QString const& mapPath, QWidget *parent = nullptr);

signals:

private:
    std::unique_ptr<QLabel> m_mainLabel;
    std::unique_ptr<node> m_centreNode;

};

#endif // MAPVIEWER_H
