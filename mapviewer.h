#ifndef MAPVIEWER_H
#define MAPVIEWER_H
#include <vector>
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
#include "mapdatamanager.h"
#include "node.h"
#include "nodedatamanager.h"

/*
 * This class is in charge of displaying the map and in-map mouse events
*/
class MapViewer : public QWidget
{
    Q_OBJECT
public:
    explicit MapViewer(QString const& mapPath, QWidget *parent = nullptr);

    void saveActiveMap();

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

    void addChildForSelectedNode();
    void deleteSelectedNode();

    void updataDataForNode(Node* node);

signals:
    void nodeSelectionChanged(bool active);

private:
    std::unique_ptr<QLabel> m_mainLabel;
    std::vector<Node*>m_nodes;
    QPoint m_lastMousePoint;
    Node *m_grabbedNode;
    QPoint m_grabbedHotSpot;
    MapDataManager m_currentMap;
};

#endif // MAPVIEWER_H
