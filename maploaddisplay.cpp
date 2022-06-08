#include "maploaddisplay.h"
#include <QString>
MapLoadDisplay::MapLoadDisplay(QWidget *parent) : QWidget(parent)
{
    m_layout = std::make_unique<QHBoxLayout>(this);// this keyword is important, we tell the widget the parent withn be showed

    Mapmanager mapman;
    std::vector<mapData> maps = mapman.getAvailableMaps();

    m_listWidget = std::make_unique<QListWidget>();
    m_listWidget->setMinimumWidth(100);
    m_listWidget->setMinimumHeight(100);

    for (mapData& map : maps)
    {
        m_listWidget->addItem(map.mapSubject);
    }
    m_layout->addWidget(m_listWidget.get());

    /*Note: You should only use connect(...) if you are inside a class that inherits from QObject, in the case of main you should use QObject::connect(...)*/
    QObject::connect(m_listWidget.get(), SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(on_itemDoubleClicked(QListWidgetItem*)));
}

void MapLoadDisplay::on_itemDoubleClicked(QListWidgetItem *item)
{
    qInfo("item double clicked: %s", qUtf8Printable(item->text()));
}
