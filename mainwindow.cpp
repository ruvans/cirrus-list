#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":resources/cirrusmap_appicon.ico"));

    //connect buttons and slots
    //node that some older buttons have been set via the form
    connect(ui->addChildButton, SIGNAL(clicked()), this, SLOT(on_createChildNode_clicked()));
    connect(ui->deleteNodeButton, SIGNAL(clicked()), this, SLOT(on_deleteNode_clicked()));
    //menu bar buttons
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(on_newButton_clicked()));
    connect(ui->actionLoad, SIGNAL(triggered()), this, SLOT(on_loadButton_clicked()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(on_saveButton_clicked()));

    //https://doc.qt.io/qt-6/stylesheet-examples.html
    setStyleSheet(
                "#centralwidget { background-color: #a2d2ff; }"
                "QPushButton { background-color: #cdb4db; }"
                "QStatusBar { background-color: #7BB1E4; }"
                "QMenuBar { background-color: #7BB1E4; }"
                "QWidget#scrollAreaWidgetContents { background-color: transparent; }"//otherwise hard to colour main widgets
                "QScrollArea { background-color: #BDDFFE; }"
                "QWidget#mapArea { background: qlineargradient( x1:0 y1:0, x2:0.1 y2:1, stop:0 #bddffe, stop:0.7 #bdc4fe, stop:1 #c5baed); }"
                );
//https://doc.qt.io/qt-6/qgradient.html#CoordinateMode-enum
    //background-image: linear-gradient(120deg, #e0c3fc 0%, #8ec5fc 100%);
    //linear-gradient(90deg, rgba(238,174,202,1) 0%, rgba(189,223,254,1) 48%, rgba(189,254,207,1) 100%)
    showMapLoadDisplay();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_newButton_clicked()
{
    bool ok = false;
    QString text = QInputDialog::getText(/*parent*/this,
                                         /*dialog title*/tr("New CirrusList"),
                                         /*label text*/tr("CirrusList subject:"),
                                         /*text display mode*/QLineEdit::Normal,
                                         /*sample text*/tr(""), &ok);

    if (ok && text.isEmpty() == false)
    {
         //open new
        qInfo("Creating new cirrusList for: %s", qUtf8Printable(text));
        Mapmanager mapman;
        QString newMap = mapman.makeNewMap(text);

        if (newMap.isEmpty() == false)
        {
            showMap(newMap);
        }
    }
}

void MainWindow::showMapLoadDisplay()
{
    m_mapLoadDisplay = std::make_unique<MapLoadDisplay>();
    m_mapLoadDisplay->resize(200, 200);

    QObject::connect(m_mapLoadDisplay.get(), &MapLoadDisplay::mapItemClicked, this, &MainWindow::on_mapToLoadChosen);
    ui->scrollArea->setWidget(m_mapLoadDisplay.get());
    ui->scrollArea->show();
}

void MainWindow::on_loadButton_clicked()
{
    showMapLoadDisplay();
}

void MainWindow::on_saveButton_clicked()
{
    if (m_mapViewer != nullptr)
    {
        m_mapViewer->saveActiveMap();
    }
}

void MainWindow::on_mapToLoadChosen(QString mapFile)
{
    if (mapFile.isEmpty() == false)
    {
        showMap(mapFile);
    }
}

void MainWindow::on_nodeSelectionChanged(bool active)
{
    ui->nodeTools->setEnabled(active);
}

void MainWindow::on_createChildNode_clicked()
{
    qInfo("Creating child node");
    if (m_mapViewer != nullptr)
    {
        m_mapViewer->addChildForSelectedNode();
    }
}

void MainWindow::on_deleteNode_clicked()
{
    qInfo("Node murder");
    if (m_mapViewer != nullptr)
    {
        m_mapViewer->deleteSelectedNode();
    }
}

void MainWindow::showMap(QString mapPath)
{
    QFile file(mapPath);
    if (!file.exists())
    {
        QMessageBox::warning(this,"Error MW1","Cannot find file at path: " + mapPath);
        return;
    }

    m_mapViewer.release();
    m_mapViewer = std::make_unique<MapViewer>(mapPath, ui->scrollArea);
    m_mapViewer->resize(200, 200);
    //QObject::connect(m_mapLoadDisplay.get(), &MapLoadDisplay::mapItemClicked, this, &MainWindow::on_mapToLoadChosen);
    QObject::connect(m_mapViewer.get(), &MapViewer::nodeSelectionChanged, this, &MainWindow::on_nodeSelectionChanged);
    ui->scrollArea->takeWidget();
    ui->scrollArea->setWidget(m_mapViewer.get());
    ui->scrollArea->show();

}



