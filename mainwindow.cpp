#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mapmanager.h"

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

    if (ok && !text.isEmpty())
    {
         //open new
        qInfo("Creating new cirrusList for: %s", qUtf8Printable(text));
        Mapmanager mapman;
        mapman.makeNewMap(text);

    }

}

