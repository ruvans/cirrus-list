#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //m_mapLoadDisplay = new MapLoadDisplay(ui->scrollArea);
    m_mapLoadDisplay.setBackgroundRole(QPalette::Dark);
    m_mapLoadDisplay.resize(200, 200);

    QObject::connect(&m_mapLoadDisplay, &MapLoadDisplay::mapItemClicked, this, &MainWindow::on_mapToLoadChosen);

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

void MainWindow::on_loadButton_clicked()
{
    //todo: m_mapLoadDisplay->update();
    ui->scrollArea->setWidget(&m_mapLoadDisplay);
    ui->scrollArea->show();
}

void MainWindow::on_mapToLoadChosen(QString mapFile)
{
    if (mapFile.isEmpty() == false)
    {
        showMap(mapFile);
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

    //MapListViewer *mapListViewer = new MapListViewer(mapPath, ui->scrollArea);
    m_mapViewer.release();
    m_mapViewer = std::make_unique<MapListViewer>(mapPath, ui->scrollArea);
    m_mapViewer->setBackgroundRole(QPalette::Dark);
    m_mapViewer->resize(200, 200);
    ui->scrollArea->takeWidget();
    ui->scrollArea->setWidget(m_mapViewer.get());
    ui->scrollArea->show();

}
