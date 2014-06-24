/*
 *  Description : Home Creator 3D
 *
 *  Auteur : Alexandre Lefranc, ÉTS (C) 2012
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QList>
#include <QTreeWidgetItem>
#include <QString>

// Constructeur
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton2D, SIGNAL(clicked()), this, SLOT(click2DView()));
    connect(ui->pushButton3D, SIGNAL(clicked()), this, SLOT(click3DView()));
    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(itemClick(QTreeWidgetItem *, int)));

    connect(ui->actionNouveau, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(ui->actionOuvrir, SIGNAL(triggered()), this, SLOT(loadFile()));
    connect(ui->actionSauvegarder, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(ui->actionQuitter, SIGNAL(triggered()), this, SLOT(quit()));

    connect(ui->actionAide, SIGNAL(triggered()), ui->widget, SLOT(help()));
    connect(ui->action_propos, SIGNAL(triggered()), this, SLOT(createAndShowAbout()));
}


// Destructeur
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::resizeEvent( QResizeEvent * event )
{

}

// Bouton "plan 2D"
void MainWindow::click2DView()
{
    ui->widget->init2D();
    ui->widget->repaint();
}

// bouton "Vue 3D"
void MainWindow::click3DView()
{
    ui->widget->init3D();
    ui->widget->repaint();
}

// Méthode appelée lorsqu'un item du menu est sélectionné
void MainWindow::itemClick(QTreeWidgetItem * item, int column)
{
    emit itemSelected(item->text(column));
}


Ui::MainWindow* MainWindow::getUi()
{
    return ui;
}

/* Méthode qui permet de redessiner le widget GL */
void MainWindow::refreshRender()
{
    ui->widget->repaint();
}

/* Méthode qui permet d'activer le bouton Vue 3D */
void MainWindow::enable3D()
{
    ui->pushButton3D->setEnabled(true);
}

/* Méthode qui permet de désactiver le bouton Vue 3D */
void MainWindow::disable3D()
{
    ui->pushButton3D->setEnabled(false);
}

/* Méthode qui permet de récupérer un nom de fichier pour charger une maison */
void MainWindow::loadFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
             "Ouvrir", "",
             "Architecture 3D (*.a3d);;Tous les fichiers (*.*)");

    if (!fileName.isEmpty())
        emit loadHouse(fileName);
}

/* Méthode qui permet de récupérer un nom de fichier pour sauvegarder une maison */
bool MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        "Enregistrer sous", "", "Architecture 3D (*.a3d)");

    if (!fileName.isEmpty())
    {
        emit saveHouse(fileName);
        return true;
    }
    else
        return false;
}

/* Méthode qui permet de mettre fin au programme */
void MainWindow::quit()
{
    int ret = QMessageBox::warning(this, "Architecture 3D",
                                   "Voulez-vous enregistrer les modifications ?",
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                   QMessageBox::Save);
    switch (ret)
    {
        case QMessageBox::Discard : qApp->quit(); break;
        case QMessageBox::Save : if (saveFile()) qApp->quit(); break;
    }
}

/* Méthode qui permet de créer un nouveau projet */
void MainWindow::newFile()
{
    disable3D();
    emit newHouse();
}

/* Méthode qui permet d'afficher la fenêtre "à propos" */
void MainWindow::createAndShowAbout()
{
    Ui::AboutDialog aboutDlg;

    QDialog aboutQDlg(this);
    aboutDlg.setupUi(&aboutQDlg);

    aboutQDlg.exec();
}

// EOF
