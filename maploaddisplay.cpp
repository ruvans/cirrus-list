#include "maploaddisplay.h"
#include <QString>
#include <QtGui>
MapLoadDisplay::MapLoadDisplay(QWidget *parent) : QWidget(parent)
{
    this->setObjectName("mapArea");
    m_descriptionLabel = std::make_unique<QLabel>("Description");
    m_dateLabel = std::make_unique<QLabel>("Date");

    m_labelLayout = std::make_unique<QHBoxLayout>();//no 'this', so m_layout will take ownership
    m_labelLayout->addWidget(m_descriptionLabel.get());
    m_labelLayout->addWidget(m_dateLabel.get());

    m_layout = std::make_unique<QVBoxLayout>(this);// 'this' keyword is important, we tell the widget the parent to be showed in
    m_layout->addLayout(m_labelLayout.get());
    Mapmanager mapman;
    std::vector<MapData> maps = mapman.getAvailableMaps();

    m_listWidget = std::make_unique<QListWidget>();
    m_listWidget->setMinimumWidth(100);
    m_listWidget->setMinimumHeight(100);

    for (MapData& map : maps)
    {
        m_mapListWidgetItems.push_back(new MapListWidgetItem(map));
        m_listItems.push_back(new QListWidgetItem);
        m_listItems.back()->setSizeHint(QSize(m_listItems.back()->sizeHint().width(), 50));
        m_listWidget->addItem(m_listItems.back());
        m_listWidget->setItemWidget(m_listItems.back(), m_mapListWidgetItems.back());
    }
    m_layout->addWidget(m_listWidget.get());

    /*Note: You should only use connect(...) if you are inside a class that inherits from QObject, in the case of main you should use QObject::connect(...)*/
    QObject::connect(m_listWidget.get(), SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(on_itemDoubleClicked(QListWidgetItem*)));
}

MapLoadDisplay::~MapLoadDisplay()
{
    for (auto item : m_listItems)
    {
        delete item;
    }
    for (auto item: m_mapListWidgetItems)
    {
        delete item;
    }
    m_listItems.clear();
    m_mapListWidgetItems.clear();
    m_labelLayout.release();
    m_layout.release();
    m_listWidget.release();
}

void MapLoadDisplay::on_itemDoubleClicked(QListWidgetItem *item)
{
    MapListWidgetItem *mapItem = dynamic_cast<MapListWidgetItem*>(m_listWidget->itemWidget(item));
    if (mapItem != nullptr)
    {
        qInfo("file to load is %s", qUtf8Printable(mapItem->m_itemMapData.mapFilename));
        emit mapItemClicked(mapItem->m_itemMapData.mapPath);
    }
}
