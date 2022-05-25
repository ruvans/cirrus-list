#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
            MapListViewer *mapListViewer = new MapListViewer(newMap, ui->scrollArea);
            mapListViewer->setBackgroundRole(QPalette::Dark);
            mapListViewer->resize(200, 200);
            ui->scrollArea->setWidget(mapListViewer);
            ui->scrollArea->show();
        }

    }

}

void MainWindow::on_loadButton_clicked()
{
    MapLoadDisplay *mapLoadDisplay = new MapLoadDisplay(ui->scrollArea);
    mapLoadDisplay->setBackgroundRole(QPalette::Dark);
    mapLoadDisplay->resize(200, 200);
    ui->scrollArea->setWidget(mapLoadDisplay);
    ui->scrollArea->show();
}
