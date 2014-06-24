/*
 *  Description : Home Creator 3D
 *
 *  Auteur : Alexandre Lefranc, ÉTS (C) 2012
 */

#ifndef POINT_H
#define POINT_H

#include "qgl.h"

// Classe Point
class Point
{
private:
    double _x, _y;
    GLuint _name;
    GLubyte _color[3];

public:
    static GLuint num;
    static GLubyte colorDefault[3];
    Point() { }
    Point(double x, double y);

    void setX(double x) { _x = x; }
    void setY(double y) { _y = y; }
    void setName(GLuint name) { _name = name; }
    void setColor(GLubyte* color) { _color[0] = color[0];
                                    _color[1] = color[1];
                                    _color[2] = color[2];
                                  }

    double getX() { return _x; }
    double getY() { return _y; }
    GLuint getName() { return _name; }

    void draw(bool withName)
    {
        if (withName)
        {
            glPushMatrix();
            glPointSize(12.0);
            glPushName(_name);
            glBegin(GL_POINTS);
                glColor3ubv(_color);
                glVertex2d(_x, _y);
            glEnd();
            glPopName();
            glPopMatrix();
        }
        else
        {
            glPushMatrix();
            glPointSize(12.0);
            glBegin(GL_POINTS);
                glColor3ubv(_color);
                glVertex2d(_x, _y);
            glEnd();
            glPopMatrix();
        }
    }
};


#endif // POINT_H
