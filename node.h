#ifndef NODE_H
#define NODE_H

#include <QWidget>
#include <QPixmap>
#include <QMessageLogger>
#include <QPainter>
#include <QPlainTextEdit>
#include <QHBoxLayout>
#include <QObject>
#include <QScrollBar>
#include <QLabel>
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
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void setText(QString newText);
    void setNodeProperties(NodeProperties);
    NodeProperties* getNodeProperties();

    /**
     * @brief addChildID Add a child ID to this nodes nodeProperties
     */
    void addChildID(int childNodeID);

    /**
     * @brief removeChildID remove a child ID from this nodes nodeProperties
     */
    void removeChildID(int childNodeID);

    void setSelected(bool newState);

    void showTextInputBox();
    void hideTextInputBox();
    void setNewText(QString newText);

signals:
    void nodePropertiesChanged(Node *node);

public slots:
    void inputTextRowsChanged();

private:
    void drawBackground();
    NodeProperties m_nodeProperties;
    bool isSelected{false};
    std::unique_ptr<QPlainTextEdit> m_textInput;
    std::unique_ptr<QHBoxLayout> m_layout;
    QLabel m_text;

};

#endif // NODE_H
