/*
 *  Description : Home Creator 3D
 *
 *  Auteur : Alexandre Lefranc, ÉTS (C) 2012
 */

#ifndef MAISON_H
#define MAISON_H

#include <QList>
#include <QtOpenGL>
#include <QObject>

#include "point.h"
#include "model.h"
#include "mobilier.h"
#include "mur.h"

#define FLOOR_NAME (1 << 15)

// Classe Maison
class Maison : public QObject
{
    Q_OBJECT
private:
    QList<Point> _points;
    Point centroid;
    QMap<GLuint, Mobilier*> _mobilier;
    QList<Mur*> _murs;
    bool _isOK, _build;
    Texture* _floorTexture;

    void drawPoints(bool withNames);

public:
    Maison();
    ~Maison();

    void init();

    void draw2D(bool withNames);
    void draw3D(bool withNames);

    void build();
    void loadTextures();
    void drawCotes(Point p1, Point p2, float textFactor);
    Point* getPointByName(GLuint name);
    Point* getFirstPoint() { return (_points.size() > 0) ? &_points[0]: NULL; }
    Point getCentroid();
    int getNumPoints() { return _points.size(); }

    void addPoint(double x, double y);



    void setOK(bool b) { _isOK = b; }
    bool isOK() { return _isOK; }
    bool addTexture(Texture* t, GLuint name);
    void drawSol(bool withName);
    Mobilier* Maison::findMobilier(GLuint name);
    void deleteObject(GLuint name);
    bool loadMaison(QString fileName, QMap<QString, Model*> & models, QMap<QString, Texture> & textures);
    bool saveMaison(QString fileName);

public slots:
    Mobilier * addMobilier(Model* models);
};

#endif // MAISON_H
