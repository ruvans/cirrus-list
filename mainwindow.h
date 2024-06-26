#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include <QMessageBox>
#include <QMessageLogger>
#include "mapmanager.h"
#include "mapviewer.h"
#include "maploaddisplay.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_newButton_clicked();
    void on_loadButton_clicked();
    void on_saveButton_clicked();
    void on_mapToLoadChosen(QString mapFile);
    //node slots
    void on_nodeSelectionChanged(bool active);
    void on_createChildNode_clicked();
    void on_deleteNode_clicked();

private:
    Ui::MainWindow *ui;
    void showMap(QString mapFile);
    void showMapLoadDisplay();
    std::unique_ptr<MapLoadDisplay> m_mapLoadDisplay;
    //MapLoadDisplay m_mapLoadDisplay;
    std::unique_ptr<MapViewer> m_mapViewer;


};
#endif // MAINWINDOW_H
