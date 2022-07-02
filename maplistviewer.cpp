#include "maplistviewer.h"

MapListViewer::MapListViewer(QString const& mapPath, QWidget *parent) : QWidget(parent)
{
    m_layout = std::make_unique<QHBoxLayout>(this);// this keyword is important, we tell the widget the parent withn be showed

    Mapmanager mapman;

    m_mainLabel = std::make_unique<QLabel>();
    m_mainLabel->setText(mapman.getMapSubject(mapPath));
    m_mainLabel->setAlignment(Qt::AlignCenter);
    m_mainLabel->setMinimumWidth(100);// You can set other properties similarly
    m_layout->addWidget(m_mainLabel.get());
}
//https://thecodeprogram.com/how-to-create-custom-widget-in-qt-c--
