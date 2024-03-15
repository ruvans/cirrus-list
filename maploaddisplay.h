#ifndef MAPLOADDISPLAY_H
#define MAPLOADDISPLAY_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QListWidget>
#include "mapmanager.h"


class MapListWidgetItem : public QWidget
{
    Q_OBJECT
public:
    MapListWidgetItem(MapData mapData)
    {
        const int itemsHeight=20;
        m_itemMapData = mapData;

        m_subjectLabel = std::make_unique<QLabel>(mapData.mapSubject);
        m_dateLabel = std::make_unique<QLabel>(mapData.lastUpdated.toString("hh:mm   dd-MMMM-yyyy"));
        m_itemLayout = std::make_unique<QHBoxLayout>(this);
        //set widget heights
        m_subjectLabel->setMaximumHeight(itemsHeight);
        m_dateLabel->setMaximumHeight(itemsHeight);
        //add to the layout
        m_itemLayout->addWidget(m_subjectLabel.get());
        m_itemLayout->addWidget(m_dateLabel.get());
        m_itemLayout->setContentsMargins(20,0,20,0);
    }
    MapData m_itemMapData;
    std::unique_ptr<QLabel> m_subjectLabel, m_dateLabel;
    std::unique_ptr<QHBoxLayout> m_itemLayout;
};

/*
 * MapLoadDisplay is in charge is showing all the maps available on the computer
*/
class MapLoadDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit MapLoadDisplay(QWidget *parent = nullptr);
    ~MapLoadDisplay();

private slots:
void on_itemDoubleClicked(QListWidgetItem *item);


public slots:
signals:
void mapItemClicked(QString mapToLoad);

private:
std::unique_ptr<QVBoxLayout> m_layout;
std::unique_ptr<QListWidget> m_listWidget;
std::vector<QListWidgetItem*> m_listItems;
std::vector<MapListWidgetItem*> m_mapListWidgetItems;
std::unique_ptr<QHBoxLayout> m_labelLayout;
std::unique_ptr<QLabel> m_descriptionLabel, m_dateLabel;
};

#endif // MAPLOADDISPLAY_H
