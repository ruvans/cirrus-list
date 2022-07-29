#include "mapviewer.h"

MapViewer::MapViewer(QString const& mapPath, QWidget *parent) : QWidget(parent)
{
    Mapmanager mapman;

    m_centreNode = std::make_unique<node>(this);
    m_centreNode->setText(mapman.getMapSubject(mapPath));
    m_centreNode->setFixedSize(200,40);

}

