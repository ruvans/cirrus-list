#ifndef NODE_H
#define NODE_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QDir>

class node : public QWidget
{
    Q_OBJECT
public:
    explicit node(QWidget *parent = nullptr);
    void setText(QString newText);

signals:

private:
    std::unique_ptr<QPixmap> m_backgroundImg;
    std::unique_ptr<QLabel> m_backgroundImgLabel;
    std::unique_ptr<QLabel> m_mainText;
};

#endif // NODE_H
