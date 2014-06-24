/*
 *  Description : Home Creator 3D
 *
 *  Auteur : Alexandre Lefranc, ÉTS (C) 2012
 */

#ifndef GLVIEWER_H
#define GLVIEWER_H

#include <QGLViewer/qglviewer.h>
#include <QPoint>
#include <QObject>

#include "maison.h"
#include "point.h"

typedef enum {MODE_2D, MODE_3D} MODE;

// Classe QGLViewer
class GLViewer : public QGLViewer
{
    Q_OBJECT

public :
    GLViewer(QWidget* parent);

    void init2D();
    void init3D();

    void draw2D(bool withNames);
    void draw3D(bool withNames);

    void setMaison(Maison* maison);
    void setObjectMoving(bool b);

    void reInit();

signals :
    void objectChangePosition(double x, double y, double z);
    void constructionOK();
    void objectChangeRotation();
    void selection(int name);
    void deleteObject();

protected :
    virtual void init();

    virtual void draw();
    virtual void drawWithNames();

    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void postSelection(const QPoint& point);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void wheelEvent(QWheelEvent* event);
    virtual QString helpString() const;

private :
    bool objectMoving;
    int _selectedName;
    MODE mode;
    Maison* _maison;
    Point futurePoint, lastPoint;
    bool finished;
    int npoints;
    Point* pointMoving;
    Qt::MouseButton _currentButton;

    void drawBackground2D();
    void drawBackground3D(); 
    void drawSky();

    void initFog();
    void initCamera3D();
    void initLighting();
};

#endif // GLVIEWER_H
