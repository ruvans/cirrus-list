#include "node.h"

Node::Node(QWidget *parent) : QWidget(parent)
{
    m_layout = std::make_unique<QHBoxLayout>(this);
    m_layout->addWidget(&m_text);

    m_text.setWordWrap(true);
    m_text.setAlignment(Qt::AlignCenter);
    m_text.setText("text here");
    m_text.setAttribute(Qt::WA_TransparentForMouseEvents);//dont let user drag the label!!

    m_minWidth = gridSize*6+1;
    m_maxWidth = gridSize*35+1;
    m_minHeight = gridSize*2+1;
    m_maxHeight = gridSize*15+1;

    setMinimumWidth(m_minWidth);
    setMinimumHeight(m_minHeight);

    //turn on mouse tracking so mouseMoveEvent works
    setMouseTracking(true);
}

void Node::setSelected(bool newSelectedState)
{
    bool newTextEntred = (newSelectedState == false) && (m_textInput != nullptr);
    if (newTextEntred)
    {
        setNewText(m_textInput->toPlainText());
        hideTextInputBox();
    }

    if (newSelectedState != isSelected)
    {
        isSelected = newSelectedState;
        repaint();
    }

}

void Node::setText(QString newText)
{
    m_nodeProperties.nodeText = newText;
    m_text.setText(newText);

    emit nodePropertiesChanged(this);
}

void Node::setNodeProperties(NodeProperties properties)
{
    m_nodeProperties = properties;
    m_text.setText(properties.nodeText);
    this->move(properties.x, properties.y);
}

NodeProperties* Node::getNodeProperties()
{
    return &m_nodeProperties;
}

void Node::drawCloudBackground()
{
    //img is 25 px, only looks correct at 20px size for some reason(???)
    const int drawnTileSize = 20;
    QPainter painter(this);
    //paint cloudy background
    const bool rootNode = m_nodeProperties.nodeID == 0;

    QString cloudBorder;
    if (rootNode)
    {
        cloudBorder = isSelected? ":resources/cloud_border_spritesheet_root_selected.png" : ":resources/cloud_border_spritesheet_root.png";
    }
    else
    {
        cloudBorder = isSelected? ":resources/cloud_border_spritesheet_selected2.png" : ":resources/cloud_border_spritesheet2.png";
    }
    QPixmap cloudTiles(cloudBorder);

    int x = 0;
    int y = 0;
    //top left
    drawCloudTile(&painter, &cloudTiles, x, y, drawnTileSize, /*tileID*/ 4);
    //top
    x = drawnTileSize;
    for(; x < width()-(drawnTileSize*2); x+=drawnTileSize)
    {
        drawCloudTile(&painter, &cloudTiles, x, y,drawnTileSize, /*tileID*/ 0);
    }
    //top right
    drawCloudTile(&painter, &cloudTiles, x, y,drawnTileSize, /*tileID*/ 5);
    //middle - left, right and fill
    y = drawnTileSize;
    for(; y < height() - (drawnTileSize*2); y+=drawnTileSize)
    {
        drawCloudTile(&painter, &cloudTiles, 0, y,drawnTileSize, /*tileID*/ 2); // left bumps
        drawCloudTile(&painter, &cloudTiles, x, y,drawnTileSize, /*tileID*/ 3); // right bumps
        for (int xi = drawnTileSize; xi < width()-(drawnTileSize*2); xi+=drawnTileSize)
        {
            drawCloudTile(&painter, &cloudTiles, xi, y,drawnTileSize, /*tileID*/ 8); //fill
        }
    }
    //bottom left
    x = 0;
    y = std::min(y, height() - drawnTileSize);
    drawCloudTile(&painter, &cloudTiles, x, y,drawnTileSize, /*tileID*/ 7);
    x = drawnTileSize;
    //bottom row
    for(; x < width()-(drawnTileSize*2); x+=drawnTileSize)
    {
        drawCloudTile(&painter, &cloudTiles, x, y,drawnTileSize, /*tileID*/ 1);
    }
    //bottom right
    drawCloudTile(&painter, &cloudTiles, x, y,drawnTileSize, /*tileID*/ 6);

}

void Node::drawCloudTile(QPainter* painter, QPixmap* pixmap, int x, int y, int drawnTileSize, int tileNum)
{
    int spriteTileSize = 25;
    painter->drawPixmap(x,y,drawnTileSize, drawnTileSize,
                       *pixmap,
                       spriteTileSize*tileNum, 0,
                       spriteTileSize, spriteTileSize);

}

void Node::paintEvent(QPaintEvent* /*event*/)
{
    drawCloudBackground();
}

void Node::moveEvent(QMoveEvent */*event*/)
{
    //note: this is also triggered on widget initialisation
    m_nodeProperties.x = this->x();
    m_nodeProperties.y = this->y();
}

 void Node::mouseDoubleClickEvent(QMouseEvent */*event*/)
 {
     showTextInputBox();
 }

 void Node::showTextInputBox()
 {
     m_textInput.release();
     m_textInput = std::make_unique<QPlainTextEdit>();
     m_textInput->setWordWrapMode(QTextOption::WordWrap);

     m_layout->removeWidget(&m_text);
     m_layout->addWidget(m_textInput.get());
     m_textInput->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
     m_textInput->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
     m_textInput->setWordWrapMode(QTextOption::WordWrap);
     m_textInput->appendPlainText(m_text.text());
     m_textInput->moveCursor(QTextCursor::End);
     m_textInput->setFocus();
     QObject::connect(m_textInput->verticalScrollBar(), &QScrollBar::rangeChanged, this, &Node::inputTextRowsChanged);
 }

 void Node::hideTextInputBox()
 {
     qInfo("attempting to remove text widget");
     m_textInput->hide();
     m_layout->removeWidget(m_textInput.get());
     m_layout->addWidget(&m_text);
     m_textInput.release();
 }

void Node::setNewText(QString newText)
{
    if (newText.isEmpty() == false)
    {
        setText(m_textInput->toPlainText());
    }
}

void Node::inputTextRowsChanged()
{
    qInfo("input text changed");
    int lineCount = m_textInput->document()->documentLayout()->documentSize().height();
    if (lineCount > 1)
    {
        //resize box so user can see all their wonderful words
        //rightwards
        if (width() < m_maxWidth/2 )
        {
            resizeNode(ResizeSide::right, width() + gridSize);
        }
        else
        {
            //downwards
            resizeNode(ResizeSide::bottom, height() + gridSize);
        }
    }
}

void Node::addChildID(int childNodeID)
{
    //welcome my lil baby
    m_nodeProperties.children.push_back(childNodeID);
}

void Node::removeChildID(int childNodeID)
{
    //begone unwanted child!
    int index = 0;
    for (int id : m_nodeProperties.children)
    {
        if (id == childNodeID)
        {
            m_nodeProperties.children.erase(std::next(m_nodeProperties.children.begin(), index));
        }
        index++;
    }
}

bool Node::mouseHoveringInResizeGrabSpace(QPoint pos)
{
    bool mouseOverGrabbyRight(pos.y() > height() - m_resizeGrabMargin);
    bool mouseOverGrabbyBottom(pos.x() > width() - m_resizeGrabMargin);
    bool mouseOverGrabbyLeft(pos.y() < m_resizeGrabMargin);
    bool mouseOverGrabbyTop(pos.x() < m_resizeGrabMargin);
    return mouseOverGrabbyRight || mouseOverGrabbyBottom || mouseOverGrabbyLeft || mouseOverGrabbyTop;
}

Node::ResizeSide Node::getResizeSide(QPoint pos)
{
    ResizeSide side(none);
    if (pos.y() > height() - m_resizeGrabMargin)
    {
        side = bottom;
    }
    else if (pos.x() > width() - m_resizeGrabMargin)
    {
        side = right;
    }
    else if (pos.y() < m_resizeGrabMargin)
    {
        side = top;
    }
    else if (pos.x() < m_resizeGrabMargin)
    {
        side = left;
    }
    return side;
}

void Node::setResizeCursor(ResizeSide side)
{
    switch (side) {
    case bottom:
    case top:
    {
        setCursor(QCursor(Qt::SizeVerCursor));
        break;
    }
    case left:
    case right:
    {
        setCursor(QCursor(Qt::SizeHorCursor));
        break;
    }

    case bottomright: break;//todo case bottomright: Qt::SizeFDiagCursor
    case none: break;
    }
}

bool Node::grabDistanceAchieved(QPoint pos)
{
    int grabDistY = std::abs(m_dragStart.y() - pos.y());
    int grabDistX = std::abs(m_dragStart.x() - pos.x());
    return (grabDistX > m_dragMargin) || (grabDistY > m_dragMargin) ;
}

void Node::mouseMoveEvent(QMouseEvent *event)
{
    if (m_mouseDown && resizeEnabled)
    {
        switch(sideClicked)
        {
        case bottom:
            resizeNode(sideClicked, event->pos().y());
            break;
        case right:
            resizeNode(sideClicked, event->pos().x());
            break;
        case top:
            resizeNode(sideClicked, event->pos().y());
            break;
        case left:
            resizeNode(sideClicked, event->pos().x());
            break;
        case bottomright: break;//todo
        case none: break;
        }
    }
    else if (mouseHoveringInResizeGrabSpace(event->pos()))
    {
        ResizeSide side = getResizeSide(event->pos());
        setResizeCursor(side);
    }
    else
    {
        if (m_mouseDown)
        {
            if (resizeEnabled == false && m_moving == false && grabDistanceAchieved(event->pos()))
            {
                m_moving = true;
                emit signalDragInitiation(this, event->position().toPoint());
            }
        }
        else
        {
            setCursor(QCursor(Qt::ArrowCursor));
            resizeEnabled = false;
        }
    }
}


void Node::mousePressEvent(QMouseEvent *event)
{
    qInfo("mouse down");
    bool leftButtonPressed(event->button() == Qt::LeftButton);
    if (leftButtonPressed)
    {
        qInfo("left button");
        m_mouseDown = true;
        if (mouseHoveringInResizeGrabSpace(event->pos()))
        {  //resizing down
            qInfo("resize");
            resizeEnabled = true;
            sideClicked = getResizeSide(event->pos());
        }
        else
        {
            qInfo("node clicked");
            emit signalNodeClicked(this);
            setSelected(true);
            m_dragStart = event->position().toPoint();
        }
    }
}

void Node::mouseReleaseEvent(QMouseEvent */*event*/)
{
    if (resizeEnabled)
    {
        snapResizeToGrid();
    }
    resizeEnabled = false;
    resetMoveCheckers();
}

void Node::resetMoveCheckers()
{
    m_mouseDown = false;
    m_moving = false;
    m_dragStart.setX(0);
    m_dragStart.setY(0);
}

void Node::resizeNode(ResizeSide side, int pos)
{
    int delta(0), newHeight(height()), newWidth(width());
    int newY(y()), newX(x());
    bool sizeInBounds(false);

    switch (side)
    {
    case top:
        delta = pos;
        newY = y() + delta;
        newHeight = height() - delta;
        sizeInBounds = newHeight > m_minHeight &&  newHeight < m_maxHeight;
        break;
    case bottom:
        delta = height() - pos;
        newHeight = height() - delta;
        sizeInBounds = newHeight > m_minHeight &&  newHeight < m_maxHeight;
        break;
    case left:
        delta = pos;
        newX = x() + delta;
        newWidth = width() - delta;
        sizeInBounds = newWidth > m_minWidth &&  newWidth < m_maxWidth;
        break;
    case right:
        delta = width() - pos;
        newWidth = width() - delta;
        sizeInBounds = newWidth > m_minWidth &&  newWidth < m_maxWidth;
        break;
    case bottomright: break;//todo
    case none: break;
    }

    if (sizeInBounds)
    {
        setGeometry(newX, newY, newWidth, newHeight);
        m_nodeProperties.height = height();
        m_nodeProperties.width = width();
        m_nodeProperties.x = newX;
        m_nodeProperties.y = newY;
    }

    emit nodeWasResized();
}


void Node::snapResizeToGrid()
{
    int wx = width() / gridSize;
    int hx = height() / gridSize;
    int widthToGrid = (wx * gridSize) +1;
    int heightToGrid = (hx * gridSize) +1;
    resize(widthToGrid, heightToGrid);
    m_nodeProperties.height = height();
    m_nodeProperties.width = width();
    emit nodeWasResized();
}
