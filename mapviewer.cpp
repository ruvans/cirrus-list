#include "mapviewer.h"

MapViewer::MapViewer(QString const& mapPath, QWidget *parent) : QWidget(parent)
{
    Mapmanager mapman;

    m_centreNode = std::make_unique<Node>(this);
    m_centreNode->setText(mapman.getMapSubject(mapPath));
    m_centreNode->setFixedSize(200,40);
    setAcceptDrops(true);
}


void MapViewer::mousePressEvent(QMouseEvent *event)
{
    qInfo("mousePressEvent");
    QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
    if (child == nullptr)
    {
        return;
    }

    if (event->button() == Qt::LeftButton)
    {
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;
        mimeData->setText("application/x-cirrusmap");
        QPixmap tempImg(":resources/cloudbackground.jpg");
        drag->setMimeData(mimeData);
        drag->setPixmap(tempImg);
        drag->exec();
    }

}

void MapViewer::mouseReleaseEvent(QMouseEvent *event)
{
    qInfo("mouse released");
}

void MapViewer::mouseMoveEvent(QMouseEvent *event)
{
    qInfo("mouseMoveEvent");
}

void MapViewer::dragMoveEvent(QDragMoveEvent *event)
{
    //qInfo("dragMoveEvent");
    event->acceptProposedAction();
}

void MapViewer::dropEvent(QDropEvent *event)
{
    qInfo("dropEvent");
    QPointF newLocation = event->position();
    m_centreNode->move(newLocation.x() , newLocation.y());

}


void MapViewer::dragEnterEvent(QDragEnterEvent *event)
{
    qInfo("dragEnterEvent");
    event->acceptProposedAction();
}


//https://stackoverflow.com/questions/18299077/dragging-a-qwidget-in-qt-5
//https://doc.qt.io/qt-5/qtwidgets-draganddrop-fridgemagnets-example.html
