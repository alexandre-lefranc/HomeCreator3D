/*
 *  Description : Home Creator 3D
 *
 *  Auteur : Alexandre Lefranc, ÉTS (C) 2012
 */

#include "mobilier.h"

#include <QtOpenGL>

GLuint Mobilier::num = MOBILIER_START_NAME;

// Constructeur de la classe
Mobilier::Mobilier(Model* model)
{
    _model = model;
    _x = 0.0;
    _y = 0.05;
    _z = 0.0;
    _rotation = 0;
    _name = num++;
}

// Destructeur de la classe
Mobilier::~Mobilier()
{

}

// Méthode qui permet de dessiner l'objet
void Mobilier::draw(bool withName)
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();

        glTranslatef(_x, 0.05, _z);
        glRotated(((double)_rotation)*90.0, 0.0, 1.0, 0.0);

        if (withName)
            glPushName(_name);

        _model->draw();

        if (withName)
            glPopName();

    glPopMatrix();
    glPopAttrib();
}

/* Méthode qui permet de faire pivoter l'objet de 90° */
void Mobilier::changeRotation()
{
    _rotation = (_rotation + 1) % 4;
}

/* Méthode qui permet de positionner l'objet */
void Mobilier::setPosition(double x, double y, double z)
{
    _x = x;
    _y = y;
    _z = z;
}

// EOF
