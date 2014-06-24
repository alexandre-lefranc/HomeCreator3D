/*
 *  Description : Home Creator 3D
 *
 *  Auteur : Alexandre Lefranc, ÉTS (C) 2012
 */

#include "mur.h"

/* Fonction qui renvoie la distance entre deux points */
inline double static calcDist(Point p1, Point p2)
{
    return sqrt(((p2.getX()-p1.getX())*(p2.getX()-p1.getX())) +
                (p2.getY()-p1.getY())*(p2.getY()-p1.getY()));
}

GLdouble Mur::_hauteur = 2.3;
GLdouble Mur::_epaisseur = 0.3;
GLuint Mur::num = MUR_NAME_START;

// Constructeur
Mur::Mur(Point* p1, Point* p2)
{
    _names[EXTERIEUR] = num++;
    _names[INTERIEUR] = num++;

    _textures[EXTERIEUR] = NULL;
    _textures[INTERIEUR] = NULL;

    _p1 = p1;
    _p2 = p2;
}


// Méthode qui permet de dessiner un mur
void Mur::draw(bool withNames)
{
    int num_repeat_txt = 2;
/*
    double d = calcDist(*_p1, *_p2);
    num_repeat_txt = (int) ceil(d/2.0);

    qDebug() << d << ";"<<num_repeat_txt;
*/

/*
    double d = calcDist(*_p1, *_p2);
    num_repeat_txt = (int) ceil(d/2.0);

    qDebug() << d << ";"<<num_repeat_txt;
*/

    GLdouble _x1, _x2;
    GLdouble _z1, _z2;

    _x1 = _p1->getX();
    _x2 = _p2->getX();

    _z1 = _p1->getY();
    _z2 = _p2->getY();

    GLdouble f1 = 1.0 + _epaisseur/10.0;
    GLdouble f2 = 1.0 - _epaisseur/10.0;
/*
    Point a(_x1*f1, _z1*f1), b(_x1*f2, _z1*f2);

    double e = calcDist(a, b);

    if (e != _epaisseur)
    {
        f1 = f2-((0.3)/(sqrt(_x1*_x1+_z1*_z1)))-0.0001;

        Point c(_x1*f1, _z1*f1), d(_x1*f2, _z1*f2);
        e = calcDist(c, d);
        qDebug() << e;
    }*/

    // Calcul des normales
    double nd;

    // normale face exterieur
    GLdouble nx1 = -_z2*f1+_z1*f1;
    GLdouble nz1 = (_x2*f1-_x1*f1);

    nd = sqrt(nx1*nx1+nz1*nz1);

    nx1 = nx1/nd;
    nz1 = nz1/nd;

    // normale face interieur
    GLdouble nx2 = -_z2*f2+_z1*f2;
    GLdouble nz2 = (_x2*f2-_x1*f2);

    nd = sqrt(nx2*nx2+nz2*nz2);

    nx2 = -nx2/nd;
    nz2 = -nz2/nd;

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();

        glDisable(GL_NORMALIZE);
        glShadeModel(GL_SMOOTH);
        glColor4ub(200, 200, 200, 255);

        // Matériaux
        GLfloat mat_Ks[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
        GLfloat mat_Ka[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
        GLfloat mat_Kd[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
        GLfloat mat_Ke[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_Ks);
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_Ka);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_Kd);
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_Ke);
        glMaterialf(GL_FRONT, GL_SHININESS, 100.0f);

        if (withNames) glPushName( MUR_NAME_START-1);
        if (withNames) glLoadName(_names[EXTERIEUR]);

        if (_textures[EXTERIEUR] != NULL)
        {
            glEnable(GL_TEXTURE_2D);
            _textures[EXTERIEUR]->bind();
        }

        // Face gauche (Exterieur)
        glBegin(GL_QUADS);
            glNormal3d(nx1, 0.0f, nz1);            
            glTexCoord2i(0,1*num_repeat_txt);
            glVertex3d(_x2*f1, _hauteur, _z2*f1);
            glTexCoord2i(1*num_repeat_txt,1*num_repeat_txt);
            glVertex3d(_x1*f1, _hauteur, _z1*f1);
            glTexCoord2i(1*num_repeat_txt,0);
            glVertex3d(_x1*f1,     0.0f, _z1*f1);
            glTexCoord2i(0,0);
            glVertex3d(_x2*f1,     0.0f, _z2*f1);
        glEnd();

        glDisable(GL_TEXTURE_2D);

        if (withNames) glLoadName(_names[INTERIEUR]);

        if (_textures[INTERIEUR] != NULL)
        {
            glEnable(GL_TEXTURE_2D);
            _textures[INTERIEUR]->bind();
        }

        // Face droite (Interieur)
        glBegin(GL_QUADS);
            glNormal3d(nx2, 0.0f, nz2);
            glTexCoord2i(1*num_repeat_txt,1*num_repeat_txt); glVertex3d(_x2*f2, _hauteur, _z2*f2);
            glTexCoord2i(0,1*num_repeat_txt); glVertex3d(_x1*f2, _hauteur, _z1*f2);
            glTexCoord2i(0,0); glVertex3d(_x1*f2,      0.0, _z1*f2);
            glTexCoord2i(1*num_repeat_txt,0); glVertex3d(_x2*f2,      0.0, _z2*f2);
        glEnd();

        if (withNames) glPopName();


        glDisable(GL_TEXTURE_2D);
        if (!withNames)
        {
            // Face du haut
            glBegin(GL_QUADS);
                glNormal3f(0.0f, 1.0f, 0.0f);
                glVertex3d(_x1*f2, _hauteur, _z1*f2);
                glVertex3d(_x1*f1, _hauteur, _z1*f1);
                glVertex3d(_x2*f1, _hauteur, _z2*f1);
                glVertex3d(_x2*f2, _hauteur, _z2*f2);
            glEnd();
        }

    glPopMatrix();
    glPopAttrib();
}

// Méthode qui permet de connaître si le nom fournit en paramètre correspond
// à une des deux faces du mur.
bool Mur::contains(GLuint name)
{
    if (name < MUR_NAME_START)
        return false;

    return (_names[EXTERIEUR] == name || _names[INTERIEUR] == name);
}

// Méthode qui permet d'ajouter une texture à une des deux faces du mur.
void Mur::setTexture(Texture* t, GLuint name)
{
    if (_names[EXTERIEUR] == name)
        _textures[EXTERIEUR] = t;
    else if (_names[INTERIEUR] == name)
        _textures[INTERIEUR] = t;
}

// Méthode qui permet d'ajouter des textures aux deux faces du mur.
void Mur::setTextures(Texture* textExt, Texture* textInt)
{
    _textures[EXTERIEUR] = textExt;
    _textures[INTERIEUR] = textInt;
}

// EOF
