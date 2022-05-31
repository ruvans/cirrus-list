#ifndef MAPLOADDISPLAY_H
#define MAPLOADDISPLAY_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QListWidget>
#include "mapmanager.h"

class MapLoadDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit MapLoadDisplay(QWidget *parent = nullptr);

signals:

private:
std::unique_ptr<QHBoxLayout> m_layout;
std::unique_ptr<QListWidget> m_listWidget;
};

#endif // MAPLOADDISPLAY_H
