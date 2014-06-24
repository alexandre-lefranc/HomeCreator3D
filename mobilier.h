/*
 *  Description : Home Creator 3D
 *
 *  Auteur : Alexandre Lefranc, ÉTS (C) 2012
 */

#ifndef MOBILIER_H
#define MOBILIER_H

#include "model.h"

#define MOBILIER_START_NAME (1 << 16)

// Classe Mobilier
class Mobilier
{
public:
    Mobilier(Model* model);
    ~Mobilier();

    void changeRotation();
    void setPosition(double x, double y, double z);
    void draw(bool withName);
    GLuint getName() { return _name; }
    double getX() { return _x; }
    double getY() { return _y; }
    double getZ() { return _z; }

    void setRotation(int rotation) { _rotation = rotation; }
    double getRotation() { return _rotation; }
    Model* getModel() { return _model; }

private:
    double _x, _y, _z;
    int _rotation;
    GLuint _name;
    Model* _model;
    static GLuint num;
};

#endif // MOBILIER_H
