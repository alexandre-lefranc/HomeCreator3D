/*
 *  Description : Home Creator 3D
 *
 *  Auteur : Alexandre Lefranc, ÉTS (C) 2012
 */

#include <QList>
#include <math.h>
#include <gl/glut.h>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>

#include "point.h"
#include "maison.h"
#include "mur.h"

/* Fonction qui renvoie la distance entre deux points */
inline double static calcDist(Point p1, Point p2)
{
    return sqrt(((p2.getX()-p1.getX())*(p2.getX()-p1.getX())) +
                (p2.getY()-p1.getY())*(p2.getY()-p1.getY()));
}


/* Fonction qui permet d'afficher du texte à la scène */
inline void glut_drawText(GLfloat x, GLfloat y, char* text, float scaleFactor)
{
    char* p = NULL;

    glPushMatrix();
        glTranslatef(x, y, 0);
        glScalef(1.0f/(600.0f*scaleFactor), 1.0f/(600.0f*scaleFactor),1.0f);
        for (p = text; *p != NULL; ++p)
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
    glPopMatrix();
}

// Constructeur de la classe
Maison::Maison()
{
    init();
}

// Destructeur de la classe
Maison::~Maison()
{
    _points.clear();

    QList<Mobilier*> mobilier = _mobilier.values();
    for (int i = 0; i < mobilier.size(); ++i)
        delete mobilier[i];

    _mobilier.clear();

    for (int i = 0; i < _murs.size(); ++i)
        delete _murs[i];

    _murs.clear();
}

// Méthode d'initialisation
void Maison::init()
{
    _isOK = false;
    _build = false;
    _floorTexture = NULL;
}

// Méthode qui permet d'ajouter un nouveau point
// dans le contour de la maison
void Maison::addPoint(double x, double y)
{
    _points.append(Point(x, y));
}


// Méthode qui renvoie le centroide de la maison
Point Maison::getCentroid()
{
    float x, y;

    for (int i = 0; i < _points.size(); ++i)
    {
        x += _points[i].getX();
        y += _points[i].getY();
    }

    centroid.setX(x/_points.size());
    centroid.setY(y/_points.size());

    return centroid;
}

// Méthode qui permet de dessiner la maison en 2D
void Maison::draw2D(bool withNames)
{
    if (withNames)
        drawPoints(true);
    else
    {
        drawPoints(false);

        if (_points.size() > 1)
        {
            glLineWidth(4.0);
            glBegin(GL_LINES);
                glColor3ub(95, 95, 95);
                int i;

                for (i = 0; i < _points.size()-1; ++i)
                {
                    glVertex2d(_points[i].getX(), _points[i].getY());
                    glVertex2d(_points[i+1].getX(), _points[i+1].getY());
                }
                if (_isOK)
                {
                    glVertex2d(_points[i].getX(), _points[i].getY());
                    glVertex2d(_points[0].getX(), _points[0].getY());
                }
            glEnd();

            for (int i = 0; i < _points.size()-1; ++i)
                drawCotes(_points[i], _points[i+1], 1.0f);

            if (_isOK)
                drawCotes(_points[_points.size()-1], _points[0], 1.0f);
        }
    }
}


// Méthode qui dessiner les points du contour de la maison
void Maison::drawPoints(bool withNames)
{
    for (int i = 0; i < _points.size(); ++i)
    {
       _points[i].draw(withNames);
    }
}

// Méthode qui affiche les cotes des murs
void Maison::drawCotes(Point p1, Point p2, float textFactor)
{
    char buf[10];
    double dist = calcDist(p1, p2);

    sprintf_s(buf, "%.2lf", dist);

    glLineWidth(1.0);
    glColor3ub(95, 95, 95);
    glut_drawText(((p1.getX()+p2.getX())/2.0)+0.1, ((p1.getY()+p2.getY())/2.0)+0.1, buf, textFactor);
}

// Méthode qui permet de dessiner la maison en 3D
void Maison::draw3D(bool withNames)
{
    QList<Mobilier*> list = _mobilier.values();

    for (int i = 0; i < list.size(); ++i)
        list[i]->draw(withNames);

    for (int i = 0; i < _murs.size(); ++i)
        _murs[i]->draw(withNames);

    drawSol(withNames);
}

// Méthode qui permet de construire les murs 3D de la maison
void Maison::build()
{
    if (_build)
        return;

    _build = true;

    double xmax, ymax, xmin, ymin;
    double mx, my;
    double x, y;

    xmax = xmin = _points[0].getX();
    ymax = ymin = _points[0].getY();

    for (int i = 0; i < _points.size(); ++i)
    {
        x = _points[i].getX();
        y = _points[i].getY();

        if (x > xmax)
            xmax = x;
        if (y > ymax)
            ymax = y;

        if (x < xmin)
            xmin = x;
        if (y < ymin)
            ymin = y;
    }

    mx = (xmax+xmin)/2.0;
    my = (ymax+ymin)/2.0;

    for (int i = 0; i < _points.size(); ++i)
    {
        _points[i].setX(_points[i].getX() - mx);
        _points[i].setY(_points[i].getY() - my);
    }

    if (_points.size() > 1)
    {        
        for (int i = 0; i < _points.size()-1; ++i)
            _murs.append(new Mur(&_points[i], &_points[i+1]));

        _murs.append(new Mur(&_points[_points.size()-1],&_points[0]));
    }
}

// Méthode qui permet d'ajouter du mobilier dans la maison
Mobilier* Maison::addMobilier(Model* model)
{
    Mobilier* m = new Mobilier(model);

    _mobilier.insert(m->getName(), m);

    return m;
}

// Méthode qui renvoie un point de la maison par son nom
Point* Maison::getPointByName(GLuint name)
{
    for (int i = 0; i < _points.size(); ++i)
        if (name == _points[i].getName())
            return &_points[i];

    return NULL;
}

// Méthode qui permet d'ajouter une texture au sol ou aux surfaces des murs
bool Maison::addTexture(Texture* t, GLuint name)
{
    if (name == FLOOR_NAME)
    {
        _floorTexture = t;
        return true;
    }

    if (name < MUR_NAME_START)
        return false;

    for (int i = 0; _murs.size()-1; ++i)
    {
        if (_murs[i]->contains(name))
        {
            _murs[i]->setTexture(t, name);
            return true;
        }
    }

    return false;
}

// Méthode qui permet de dessiner le sol de la maison
void Maison::drawSol(bool withName)
{
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);

    // Matériaux
    GLfloat mat_Ks[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_Ka[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat mat_Kd[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat mat_Ke[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_Ks);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_Ka);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_Kd);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_Ke);
    glMaterialf(GL_FRONT, GL_SHININESS, 20.0f);

    if (withName)
        glPushName(FLOOR_NAME);

    _floorTexture->bind();

    glBegin(GL_POLYGON);
        glNormal3f(0.0f, 1.0f, 0.0f);
        for (int i = 0; i < _points.size(); ++i)
        {
            glTexCoord2d(_points[i].getX(), _points[i].getY());
            glVertex3d(_points[i].getX(), 0.05, _points[i].getY());
        }
    glEnd();

    glDisable(GL_TEXTURE_2D);

    if (withName)
        glPopName();
}

// Méthode qui permet d'obtenir un mobilier par son nom
Mobilier* Maison::findMobilier(GLuint name)
{
    return (_mobilier.contains(name) ? _mobilier[name] : NULL);
}

// Méthode qui supprime un mobilier de la maison
void Maison::deleteObject(GLuint name)
{
    if (_mobilier.contains(name))
        _mobilier.remove(name);
}

// Méthode qui permet de charger une maison
bool Maison::loadMaison(QString fileName, QMap<QString, Model*> & models, QMap<QString, Texture> & textures)
{
    QFile file(fileName);
    QDomDocument doc;
    QDomElement house;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return false;

    if (!doc.setContent(&file))
    {
        printf("Echec lors de l'analyse du fichier.\n");
        file.close();
        return false;
    }

    file.close();

    _isOK = true;
    _build = true;

    house = doc.documentElement();

    if (house.tagName() != "house")
        return false;

    for (QDomElement child = house.firstChildElement();
         !child.isNull();
         child = child.nextSiblingElement())
    {
        if (child.tagName() == "points")
        {
            for (QDomElement points = child.firstChildElement();
                 !points.isNull();
                 points = points.nextSiblingElement())
            {
                addPoint(points.attribute("x").toDouble(), points.attribute("y").toDouble());
            }
        }
        else if (child.tagName() == "walls")
        {
            if (_points.size() > 1)
            {
                for (int i = 0; i < _points.size()-1; ++i)
                    _murs.append(new Mur(&_points[i], &_points[i+1]));

                _murs.append(new Mur(&_points[_points.size()-1], &_points[0]));
            }

            int n = 0;
            for (QDomElement walls = child.firstChildElement();
                 !walls.isNull();
                 walls = walls.nextSiblingElement())
            {
                _murs[n++]->setTextures(&textures[walls.attribute("textureExt")], &textures[walls.attribute("textureInt")]);
            }
        }
        else if (child.tagName() == "objects")
        {
            for (QDomElement objects = child.firstChildElement();
                 !objects.isNull();
                 objects = objects.nextSiblingElement())
            {
                Mobilier * m = addMobilier(models[objects.attribute("model")]);
                m->setPosition(objects.attribute("x").toDouble(),
                               objects.attribute("y").toDouble(),
                               objects.attribute("z").toDouble());
                m->setRotation(objects.attribute("rotation").toInt());
            }
        }
        else if (child.tagName() == "floors")
        {
            QDomElement floors = child.firstChildElement();

            if (!floors.isNull() && floors.tagName() == "floor")
                _floorTexture = &textures[floors.attribute("texture")];
        }
    }

    return true;
}

// Méthode qui permet de sauvegarder une maison
bool Maison::saveMaison(QString fileName)
{
    QFile file(fileName);
    QDomDocument doc;
    QDomElement house;

    QDomNode xmlNode = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    house = doc.createElement("house");

    doc.appendChild(xmlNode);
    doc.appendChild(house);

    // Points
    QDomElement points = doc.createElement("points");

    for (int i = 0; i < _points.size(); ++i)
    {
         QDomElement ePoint = doc.createElement("point");
         ePoint.setAttribute("x", _points[i].getX());
         ePoint.setAttribute("y", _points[i].getY());
         points.appendChild(ePoint);
    }

    house.appendChild(points);

    // Textures
    QDomElement walls = doc.createElement("walls");

    for (int i = 0; i < _murs.size(); ++i)
    {
        QDomElement eWall = doc.createElement("wall");

        Texture * t = _murs[i]->getTextureExt();
        eWall.setAttribute("textureExt", (t == NULL) ? "" : t->getFileName());

        t = _murs[i]->getTextureInt();
        eWall.setAttribute("textureInt", (t == NULL) ? "" : t->getFileName());

        walls.appendChild(eWall);
    }

    house.appendChild(walls);

    // Objects
    QDomElement objects = doc.createElement("objects");

    QList<Mobilier*> mobiliers = _mobilier.values();

    for (int i = 0; i < mobiliers.size(); ++i)
    {
        QDomElement eObject = doc.createElement("object");

        eObject.setAttribute("x", mobiliers[i]->getX());
        eObject.setAttribute("y", mobiliers[i]->getY());
        eObject.setAttribute("z", mobiliers[i]->getZ());

        eObject.setAttribute("rotation", mobiliers[i]->getRotation());
        eObject.setAttribute("model", mobiliers[i]->getModel()->getModelName());

        objects.appendChild(eObject);
    }

    house.appendChild(objects);

    // Floor
    QDomElement floors = doc.createElement("floors");
    QDomElement eFloor = doc.createElement("floor");

    eFloor.setAttribute("texture", _floorTexture->getFileName());

    floors.appendChild(eFloor);
    house.appendChild(floors);

    /*****/
    if (!file.open(QIODevice::WriteOnly))
    {
       printf("Erreur lors de l'écriture\n");
       return false;
    }

    // On écrit dans le fichier
    QTextStream ts(&file);
    ts << doc.toString();
    file.close();

    return true;
}


// EOF
