/*
 *  Description : Home Creator 3D
 *
 *  Auteur : Alexandre Lefranc, ÉTS (C) 2012
 */

#ifndef MUR_H
#define MUR_H

#include <QtOpenGL>

#include "texture.h"
#include "point.h"

#define MUR_NAME_START (1 << 18)

enum TYPE_SURFACE {EXTERIEUR,INTERIEUR};

// Classe Mur
class Mur
{
private :
    Point *_p1, *_p2;
    GLuint _names[2];
    Texture* _textures[2];

    static GLdouble _hauteur;
    static GLdouble _epaisseur;
    static GLuint num;

public :
    Mur(Point * p1, Point * p2);
    void draw(bool withNames);
    bool contains(GLuint name);
    void setTexture(Texture* t, GLuint name);
    void setTextures(Texture* textExt, Texture* textInt);
    Texture* getTextureExt() { return _textures[EXTERIEUR]; }
    Texture* getTextureInt() { return _textures[INTERIEUR]; }
};

#endif // MUR_H
