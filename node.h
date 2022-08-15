#ifndef NODE_H
#define NODE_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QDir>
#include <QMouseEvent>
#include <QDrag>
#include <QByteArray>
#include <QDataStream>
#include <QMessageLogger>

struct NodeProperties
{
    int x;
    int y;
    int width;
    int height;
};

class Node : public QWidget
{
    Q_OBJECT
public:
    explicit Node(QWidget *parent = nullptr);
    void setText(QString newText);
/*
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    */
signals:

private:
    std::unique_ptr<QPixmap> m_backgroundImg;
    std::unique_ptr<QLabel> m_backgroundImgLabel;
    std::unique_ptr<QLabel> m_mainText;
    NodeProperties m_nodeProperties;
    QPoint m_lastMousePoint;
};

#endif // NODE_H
