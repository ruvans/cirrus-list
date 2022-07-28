#ifndef MAPVIEWER_H
#define MAPVIEWER_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

#include "mapmanager.h"

class MapViewer : public QWidget
{
    Q_OBJECT
public:
    explicit MapViewer(QString const& mapPath, QWidget *parent = nullptr);

signals:

private:
    std::unique_ptr<QHBoxLayout> m_layout;
    std::unique_ptr<QLabel> m_mainLabel;

};

#endif // MAPVIEWER_H
