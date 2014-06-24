/*
 *  Description : Home Creator 3D
 *
 *  Auteur : Alexandre Lefranc, ÉTS (C) 2012
 */

#ifndef HOMECREATOR3D_H
#define HOMECREATOR_H

#include <QObject>
#include <windows.h>
#include <Qgl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "mainwindow.h"
#include "mobilier.h"
#include "maison.h"
#include "texture.h"

/* Classe principale de l'application */
class HomeCreator3D : public QObject
{
    Q_OBJECT

private :
    MainWindow window;
    Maison* maison;
    QMap<QString, Model*> models;
    QMap<QString, Texture> textures;

    Mobilier* objectSelected;
    Texture* textureSelected;

    void loadModels();
    void createConnection();
    void loadTextures();

private slots :
    void itemSelected(QString name);
    void objectMove(double x, double y, double z);
    void objectRotate();
    void enable3D();
    void selection(int name);
    void deleteObject();
    void reInit();
    bool loadMaison(QString fileName);
    bool saveMaison(QString fileName);

public :
    HomeCreator3D();

    void show();
};

#endif // HOMECREATOR_H
