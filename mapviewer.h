#ifndef MAPVIEWER_H
#define MAPVIEWER_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QByteArray>
#include <QDataStream>
#include <QPainter>
#include <QMessageLogger>


#include "mapmanager.h"
#include "node.h"

class MapViewer : public QWidget
{
    Q_OBJECT
public:
    explicit MapViewer(QString const& mapPath, QWidget *parent = nullptr);

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;



signals:

private:
    std::unique_ptr<QLabel> m_mainLabel;
    std::unique_ptr<Node> m_centreNode;
    QPoint m_lastMousePoint;

};

#endif // MAPVIEWER_H
