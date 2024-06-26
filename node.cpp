#include "node.h"

Node::Node(QWidget *parent) : QWidget(parent)
{
    m_layout = std::make_unique<QHBoxLayout>(this);
    m_layout->addWidget(&m_text);

    m_text.setWordWrap(true);
    m_text.setAlignment(Qt::AlignCenter);
    m_text.setText("text here");
    m_text.setAttribute(Qt::WA_TransparentForMouseEvents);//dont let user drag the label!!
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
    move(properties.x, properties.y);
}

NodeProperties* Node::getNodeProperties()
{
    return &m_nodeProperties;
}

void Node::drawBackground()
{

}

void Node::paintEvent(QPaintEvent* /*event*/)
{
    const QRect nodeRect(0,0,this->width(), this->height());

    QPainter painter(this);
    //paint cloudy background
    const bool rootNode = m_nodeProperties.nodeID == 0;
    QString cloudResource(":resources/cloudbackground.png");
    if (isSelected)
    {
        cloudResource = rootNode? ":resources/activecloudbackgroundmain.png" : ":resources/activecloudbackground.png";
    }
    else
    {
        cloudResource = rootNode? ":resources/cloudbackgroundmain.png" : ":resources/cloudbackground.png";
    }
    QPixmap backgroundImg(cloudResource);
    painter.drawPixmap(0,0,nodeRect.width(),nodeRect.height(), backgroundImg);

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
        int newHeight = this->height() + 12;
        this->setFixedHeight(newHeight);
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
