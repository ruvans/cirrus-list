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
        m_dateLabel = std::make_unique<QLabel>(mapData.lastUpdated);
        m_itemLayout = std::make_unique<QHBoxLayout>(this);
        //set widget heights
        m_subjectLabel->setMaximumHeight(itemsHeight);
        m_dateLabel->setMaximumHeight(itemsHeight);
        //add to the layout
        m_itemLayout->addWidget(m_subjectLabel.get());
        m_itemLayout->addWidget(m_dateLabel.get());
        m_itemLayout->setContentsMargins(20,0,20,0);
        this->setLayout(m_itemLayout.get());
        this->show();
    }
    MapData m_itemMapData;
    std::unique_ptr<QLabel> m_subjectLabel, m_dateLabel;
    std::unique_ptr<QHBoxLayout> m_itemLayout;
};



class MapLoadDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit MapLoadDisplay(QWidget *parent = nullptr);
    ~MapLoadDisplay();

signals:
private slots:
void on_itemDoubleClicked(QListWidgetItem *item);

private:
std::unique_ptr<QHBoxLayout> m_layout;
std::unique_ptr<QListWidget> m_listWidget;
};

#endif // MAPLOADDISPLAY_H