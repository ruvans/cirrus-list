#include "maploaddisplay.h"
#include <QString>
#include <QtGui>
MapLoadDisplay::MapLoadDisplay(QWidget *parent) : QWidget(parent)
{
    m_layout = std::make_unique<QHBoxLayout>(this);// this keyword is important, we tell the widget the parent withn be showed

    Mapmanager mapman;
    std::vector<MapData> maps = mapman.getAvailableMaps();

    m_listWidget = std::make_unique<QListWidget>();
    m_listWidget->setMinimumWidth(100);
    m_listWidget->setMinimumHeight(100);

    for (MapData& map : maps)
    {
        MapListWidgetItem* widgetItem= new MapListWidgetItem(map);
        QListWidgetItem* item = new QListWidgetItem;
        item->setSizeHint(QSize(item->sizeHint().width(), 50));
        m_listWidget->addItem(item);
        m_listWidget->setItemWidget(item, widgetItem);
    }
    m_layout->addWidget(m_listWidget.get());

    /*Note: You should only use connect(...) if you are inside a class that inherits from QObject, in the case of main you should use QObject::connect(...)*/
    QObject::connect(m_listWidget.get(), SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(on_itemDoubleClicked(QListWidgetItem*)));
}

MapLoadDisplay::~MapLoadDisplay()
{
}

void MapLoadDisplay::on_itemDoubleClicked(QListWidgetItem *item)
{
    MapListWidgetItem *mapItem = dynamic_cast<MapListWidgetItem*>(m_listWidget->itemWidget(item));
    if (mapItem != nullptr)
    {
        qInfo("file to load is %s", qUtf8Printable(mapItem->m_itemMapData.mapFilename));
    }
}
