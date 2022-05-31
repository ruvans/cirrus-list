#include "maploaddisplay.h"

MapLoadDisplay::MapLoadDisplay(QWidget *parent) : QWidget(parent)
{
    m_layout = std::make_unique<QHBoxLayout>(this);// this keyword is important, we tell the widget the parent withn be showed

    Mapmanager mapman;
    std::vector<mapData> maps = mapman.getAvailableMaps();

    m_mainLabel = std::make_unique<QLabel>();
    m_mainLabel->setText(maps.front().mapSubject);
    m_mainLabel->setAlignment(Qt::AlignCenter);
    m_mainLabel->setMinimumWidth(100);// You can set other properties similarly

    m_layout->addWidget(m_mainLabel.get());
}
