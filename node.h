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

    void mouseMoveEvent(QMouseEvent *event)override;
    void mousePressEvent(QMouseEvent *event)override;
    void mouseReleaseEvent(QMouseEvent *event)override;

    void setText(QString newText);
    void setNodeProperties(NodeProperties);
    NodeProperties* getNodeProperties();

    /** addChildID Add a child ID to this nodes nodeProperties */
    void addChildID(int childNodeID);

    /** removeChildID remove a child ID from this nodes nodeProperties */
    void removeChildID(int childNodeID);

    void setSelected(bool newState);

    void showTextInputBox();
    void hideTextInputBox();
    void setNewText(QString newText);

    bool resizeEnabled{false};

    /** To be called after a node drag has finished.
     * @todo I'd prefer we didn't need to do this. There must be a way to automate it
     */
    void resetMoveCheckers();

    bool isSelected{false};

    const int gridSize{20};

    /** Resize the node snapping to closest grid space*/
    void resizeToGrid(int newHeight);

    /** Is the mouse hovering over the grabby space of the node */
    bool mouseHoveringInResizeGrabSpace(int yPos);

    /** Has the mouse moved enough to be sure this is a drag gesture */
    bool grabDistanceAchieved(QPoint pos);
signals:
    void nodePropertiesChanged(Node *node);
    void signalDragInitiation(Node* node, QPoint mousePos);
    void signalNodeClicked(Node* node);

public slots:
    // Attached to the vert slider so I can find out when a
    // new line is needed while the user is writing in a textbox
    void inputTextRowsChanged();


private:
    void drawCloudBackground();
    void drawCloudTile(QPainter* painter, QPixmap* pixmap, int x, int y, int tileSize, int tileNum);
    NodeProperties m_nodeProperties;

    std::unique_ptr<QPlainTextEdit> m_textInput;
    std::unique_ptr<QHBoxLayout> m_layout;
    QLabel m_text;
    const int m_resizeGrabMargin{10};
    const int m_dragMargin{2};
    QPoint m_dragStart{0,0};
    bool m_moving{false};
    bool m_mouseDown{false};
    const int m_minimumWidth{100};
};

#endif // NODE_H
