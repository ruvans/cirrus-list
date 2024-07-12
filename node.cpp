#include "node.h"

Node::Node(QWidget *parent) : QWidget(parent)
{
    m_layout = std::make_unique<QHBoxLayout>(this);
    m_layout->addWidget(&m_text);

    m_text.setWordWrap(true);
    m_text.setAlignment(Qt::AlignCenter);
    m_text.setText("text here");
    m_text.setAttribute(Qt::WA_TransparentForMouseEvents);//dont let user drag the label!!
    setMinimumWidth(150);
    setMinimumHeight(20);
}

void Node::setSelected(bool newState)
{
    bool newTextEntred = (newState == false) && (m_textInput != nullptr);
    if (newTextEntred)
    {
        setNewText(m_textInput->toPlainText());
        hideTextInputBox();
    }

    if (newState != isSelected)
    {
        isSelected = newState;
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
}

NodeProperties* Node::getNodeProperties()
{
    return &m_nodeProperties;
}

void Node::drawCloudBackground()
{
    const int drawnTileSize = 20;
   // const QRect nodeRect(0,0,this->width(), this->height());
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
        cloudBorder = isSelected? ":resources/cloud_border_spritesheet_selected.png" : ":resources/cloud_border_spritesheet.png";
    }
    QPixmap cloudTiles(cloudBorder);
    //img is 25 px, only looks correct at 20px size for some reason(???)


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
        drawCloudTile(&painter, &cloudTiles, x, y,drawnTileSize, /*tileID*/ 3); //right bumps
        for (int xi = drawnTileSize; xi < width()-(drawnTileSize*2); xi+=drawnTileSize)
        {
            drawCloudTile(&painter, &cloudTiles, xi, y,drawnTileSize, /*tileID*/ 8); //fill
        }
    }
    //bottom left
    x = 0;
    y = std::min( y, height() - 20);
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
    qInfo("widget moved");
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
        int margin(12);
        int newHeight = this->height() + margin;
        this->setFixedHeight(newHeight);
        m_nodeProperties.height = newHeight;
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
