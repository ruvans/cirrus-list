#ifndef MAPLISTVIEWER_H
#define MAPLISTVIEWER_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

#include "mapmanager.h"

class MapListViewer : public QWidget
{
    Q_OBJECT
public:
    explicit MapListViewer(QString mapPath, QWidget *parent = nullptr);

signals:

private:
    std::unique_ptr<QHBoxLayout> m_layout;
    std::unique_ptr<QLabel> m_mainLabel;

};

#endif // MAPLISTVIEWER_H
