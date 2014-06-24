/*
 *  Description : Home Creator 3D
 *
 *  Auteur : Alexandre Lefranc, ÉTS (C) 2012
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QList>

#include "mobilier.h"
#include "ui_aboutdialog.h"

namespace Ui {
    class MainWindow;
    class AboutDialog;
}

// Classe de la fenêtre principale
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void enable3D();
    void disable3D();

    Ui::MainWindow* getUi();
    void refreshRender();

    void resizeEvent( QResizeEvent * event );

signals:
    void itemSelected(QString name);

    void newHouse();
    void loadHouse(QString fileName);
    void saveHouse(QString fileName);

private slots:
    void click3DView();
    void click2DView();

    void itemClick(QTreeWidgetItem * item, int column);

    void newFile();
    void loadFile();
    bool saveFile();

    void quit();
    void createAndShowAbout();

private:
    Ui::MainWindow* ui;
};

#endif // MAINWINDOW_H
