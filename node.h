#ifndef NODE_H
#define NODE_H

#include <QWidget>
#include <QPixmap>
#include <QMessageLogger>
#include <QPainter>
#include "nodedatamanager.h"

/* Class: Node
 * In Cirrus Map a Node is a single text cloud.
 * note: in the future there will be different types of nodes
*/
class Node : public QWidget
{
    Q_OBJECT
public:
    explicit Node(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
    void moveEvent(QMoveEvent *event) override;
    void setText(QString newText);
    void setNodeProperties(NodeProperties);
    NodeProperties* getNodeProperties();

    void setSelected(bool newState);
signals:

private:
    NodeProperties m_nodeProperties;
    bool isSelected{false};

};

#endif // NODE_H
