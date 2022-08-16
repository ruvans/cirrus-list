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
#include <QPainter>

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
    void paintEvent(QPaintEvent *event) override;


signals:

private:
    //void paintBackground();

    std::unique_ptr<QPixmap> m_backgroundImg;
    std::unique_ptr<QLabel> m_backgroundImgLabel;
    std::unique_ptr<QLabel> m_mainText;
    NodeProperties m_nodeProperties;
    QString m_nodeText;
};

#endif // NODE_H
