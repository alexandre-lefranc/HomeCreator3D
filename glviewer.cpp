/*
 *  Description : Home Creator 3D
 *
 *  Auteur : Alexandre Lefranc, ÉTS (C) 2012
 */

#include "glviewer.h"
#include "myconstraint.h"
#include <QMouseEvent>

using namespace std;
using namespace qglviewer;

// Constructeur
GLViewer::GLViewer(QWidget *parent) : QGLViewer(parent)
{
    finished = false;
    npoints = 0;
    _selectedName = -1;
    objectMoving = false;
}


void GLViewer::setMaison(Maison* maison)
{
    _maison = maison;
}


void GLViewer::reInit()
{
    finished = _maison->isOK();
    npoints = _maison->getNumPoints();

    _selectedName = -1;
    objectMoving = false;

    init();
}


void GLViewer::init()
{
    init2D();
}

/* Méthode d'initialisation 2D*/
void GLViewer::init2D()
{
    objectMoving = false;
    camera()->setType(qglviewer::Camera::ORTHOGRAPHIC);

    WorldConstraint* constraint = new WorldConstraint();
    constraint->setRotationConstraintType(AxisPlaneConstraint::FORBIDDEN);

    camera()->frame()->setConstraint(constraint);
    camera()->frame()->setWheelSensitivity(0.1f);
    camera()->frame()->setPosition(Vec(0.0,0.0,10.0));
    camera()->frame()->setRotation(Quaternion(0.0,0.0,0.0,0.0));

    setMouseBinding(Qt::ALT + Qt::LeftButton, CAMERA, NO_MOUSE_ACTION);
    setMouseBinding(Qt::LeftButton + Qt::MidButton, CAMERA, NO_MOUSE_ACTION);
    setMouseBinding(Qt::CTRL + Qt::LeftButton + Qt::MidButton, CAMERA, NO_MOUSE_ACTION);

    setMouseBinding(Qt::RightButton, CAMERA, TRANSLATE);

    setMouseBinding(Qt::LeftButton, NO_CLICK_ACTION, true);
    setMouseBinding(Qt::LeftButton, SELECT, false);

    setMouseTracking(true);

    setSceneRadius(100);

    setBackgroundColor(QColor(255, 255, 255));

    setSelectBufferSize(4);
    setSelectRegionWidth(15);
    setSelectRegionHeight(15);

    glDisable(GL_LIGHTING);
    glEnable(GL_LINE_SMOOTH);    
    glEnable(GL_BLEND);
    glDepthFunc(GL_LEQUAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

    camera()->centerScene();

    mode = MODE_2D;
}


/* Méthode draw */
void GLViewer::draw()
{
    if (mode == MODE_2D)
        draw2D(false);
    else if (mode == MODE_3D)
        draw3D(false);
}

/* Méthode draw pour la sélection */
void GLViewer::drawWithNames()
{
    if (mode == MODE_2D)
        _maison->draw2D(true);
    else if (mode == MODE_3D)
        _maison->draw3D(true);
}


/* Méthode appelée après la sélection */
void GLViewer::postSelection(const QPoint& point)
{
    bool found;
    Vec selectedPoint = camera()->pointUnderPixel(point, found);

    double x = selectedPoint.v_[0];
    double y = selectedPoint.v_[1];

    _selectedName = selectedName();

    if (mode == MODE_2D)
    {
        if (npoints > 2 && _selectedName == _maison->getFirstPoint()->getName() && !finished ) // Fin
        {
            finished = true;
            _maison->setOK(true);

            emit constructionOK();
        }
        else if (_selectedName == -1 && !finished) // Ajout de point
        {
            _maison->addPoint(x, y);
            lastPoint.setX(futurePoint.getX());
            lastPoint.setY(futurePoint.getY());              
            npoints++;
        }
        else if (_selectedName != -1 && finished) // Modification de point
        {
            GLubyte color[3] = {181, 230, 29};
            objectMoving = true;
            pointMoving = _maison->getPointByName(_selectedName);
            pointMoving->setColor(color);
        }
    }
    else if (mode == MODE_3D)
    {
        if (_selectedName != -1)
        {
            emit selection(_selectedName);
        }
    }
}

/* Méthode de détection de clique de souris */
void GLViewer::mousePressEvent(QMouseEvent* event)
{
    /*
    bool found;
    Vec selectedPoint = camera()->pointUnderPixel(event->pos(), found);
    double x = selectedPoint.v_[0];
    double y = selectedPoint.v_[1];
    */
    _currentButton = event->button();

    QGLViewer::mousePressEvent(event);
}

/* Méthode de détection lorsque la souris est en mouvement */
void GLViewer::mouseMoveEvent(QMouseEvent* event)
{
    bool found;
    Vec selectedPoint = camera()->pointUnderPixel(event->pos(), found);
    double x = selectedPoint.v_[0];
    double y = selectedPoint.v_[1];
    double z = selectedPoint.v_[2];

    if (mode == MODE_2D)
    {
        if (objectMoving)
        {
            if (pointMoving)
            {
                pointMoving->setX(x);
                pointMoving->setY(y);

                update();
            }
        }
        else
        {
            futurePoint.setX(x);
            futurePoint.setY(y);

            update();
        }
    }

    else if (mode == MODE_3D)
    {
        if (objectMoving && found && _currentButton == Qt::LeftButton)
        {
            emit objectChangePosition(x, y, z);
            update();
        }
    }

    QGLViewer::mouseMoveEvent(event);
}

/* Méthode appelée lorsque un des boutons de la souris est relaché */
void GLViewer::mouseReleaseEvent(QMouseEvent* event)
{
    if (objectMoving)
    {
        if (mode == MODE_2D)
        {
            objectMoving = false;
            _selectedName = -1;

            pointMoving->setColor(Point::colorDefault);
            pointMoving = NULL;
        }
        else if (mode == MODE_3D)
        {
            objectMoving = false;
        }
    }

    QGLViewer::mouseReleaseEvent(event);
}

/* Méthode pour dessiner la scène 2D */
void GLViewer::draw2D(bool withNames)
{
    if (!withNames)
    {
        drawBackground2D();
        _maison->draw2D(false);
        drawText(10, 20, "1 carreau = 50 cm", QFont("Courier", 10, QFont::Bold, false));

        if (!finished && npoints != 0)
        {
            glPushMatrix();
                glLineWidth(3.0);
                glBegin(GL_LINES);
                    glColor3ub(7, 74, 136);
                    glVertex2d(lastPoint.getX(), lastPoint.getY());
                    glVertex2d(futurePoint.getX(), futurePoint.getY());
                glEnd();
            glPopMatrix();
            _maison->drawCotes(lastPoint, futurePoint, camera()->frame()->position().z/10.0f);
        }
    }
    else
        _maison->draw2D(true);
}

/* Méthode pour dessiner la scène 3D */
void GLViewer::draw3D(bool withNames)
{
    if (!withNames)
    {
        // LIGHT0
        Vec cameraPos = camera()->position();
        GLfloat pos[4] = {cameraPos[0], cameraPos[1], cameraPos[2], 1.0};

        glLightfv(GL_LIGHT0, GL_POSITION, pos);
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, camera()->viewDirection());

        // LIGHT1
        GLfloat l1_pos[] = {0.0f, 2.0f, 0.0f, 1.0f};
        glLightfv(GL_LIGHT1, GL_POSITION, l1_pos);

        // DRAW
        drawSky();
        drawBackground3D();
        _maison->draw3D(false);

        //drawLight(GL_LIGHT0, 0.05);
        //drawLight(GL_LIGHT1, 0.05);

/*
        printf("%f %f\n", camera()->zNear(), camera()->zFar());

        glFogf(GL_FOG_START, camera()->zNear()+10.0f);
        glFogf(GL_FOG_END, camera()->zFar());
*/
    }
    else
        _maison->draw3D(true);
}


/* Méthode pour initialiser la scène 3D */
void GLViewer::init3D()
{
    if (mode == MODE_3D)
    {
        return;
    }

    setMouseBinding(Qt::LeftButton, CAMERA, NO_MOUSE_ACTION);
    setMouseBinding(Qt::RightButton, CAMERA, ROTATE);

    setMouseTracking(false);

    setMouseBinding(Qt::LeftButton, NO_CLICK_ACTION, true);
    setMouseBinding(Qt::LeftButton, SELECT, false);
    setSelectBufferSize(4*50);
    setSelectRegionWidth(1);
    setSelectRegionHeight(1);
/*
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
*/
/*
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
*/

    initCamera3D();
    //initFog();
    initLighting();

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glDepthFunc(GL_LEQUAL);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT,GL_DONT_CARE);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    _maison->build();

    mode = MODE_3D;
}

/* Méthode pour dessiner le quadrillage 2D */
void GLViewer::drawBackground2D()
{
    glPushMatrix();
        glLineWidth(1.0);

        glBegin(GL_LINES);
            glColor3ub(150, 150, 150);

            for (float i = -100.0f; i <= 100.0f; i += 0.5f)
            {
                glVertex2f(i, -100.0f);
                glVertex2f(i, 100.0f);

                glVertex2f(-100.0f, i);
                glVertex2f(100.0f, i);
            }
        glEnd();
    glPopMatrix();
}

/* Méthode pour dessiner le quadrillage 3D */
void GLViewer::drawBackground3D()
{
    glDisable(GL_LIGHTING);
    glLineWidth(1.0);
    glBegin(GL_LINES);
        glColor3ub(230, 230, 230);

        for (float i = -100.0f; i <= 100.0f; i += 0.5f)
        {
            glVertex3f(i, 0.0f,-100.0f);
            glVertex3f(i, 0.0f,100.0f);

            glVertex3f(-100.0f, 0.0f,i);
            glVertex3f(100.0f, 0.0f,i);
        }
    glEnd();
    glEnable(GL_LIGHTING);
}

/* Méthode pour créer un brouillard */
void GLViewer::initFog()
{
    /*
    glEnable(GL_FOG);
    GLfloat color[3] = {1.0f, 1.0f, 1.0f};
    glFogfv(GL_FOG_COLOR, color);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_DENSITY, 0.5f);
    glFogf(GL_FOG_START, 1.0f);
    glFogf(GL_FOG_END, 100.0f);
    */
}

/* Méthode pour initialiser la caméra 3D */
void GLViewer::initCamera3D()
{
    camera()->setType(qglviewer::Camera::PERSPECTIVE);

    WorldConstraint* constraint1 = new WorldConstraint();
    constraint1->setRotationConstraint(AxisPlaneConstraint::AXIS, Vec(0.0, 1.0, 0.0));

    camera()->frame()->setConstraint(new MyConstraint());
    camera()->frame()->setWheelSensitivity(0.1f);
    camera()->frame()->setOrientation(Quaternion(Vec(1.0,0.0,0.0), -3.14/8));

    camera()->centerScene();
}

/* Méthode pour dessiner la skybox */
void GLViewer::drawSky()
{
    GLubyte skymax[4] = {161, 204, 242, 255};
    GLubyte skymin[4] = {213, 226, 238, 255};

    // Taille du cube
    float t = 100.0f;

    glDisable(GL_LIGHTING);
    // Render the front quad
    glBegin(GL_QUADS);
        glColor3ubv(skymin);
        glVertex3f(  t, 0.0f, -t );
        glVertex3f( -t, 0.0f, -t );
        glColor3ubv(skymax);
        glVertex3f( -t,  t, -t );
        glVertex3f(  t,  t, -t );
    glEnd();

    // Render the left quad
    glBegin(GL_QUADS);
        glColor3ubv(skymin);
        glVertex3f(  t, 0.0f,  t );
        glVertex3f(  t, 0.0f, -t );
        glColor3ubv(skymax);
        glVertex3f(  t,  t, -t );
        glVertex3f(  t,  t,  t );
    glEnd();

    // Render the back quad
    glBegin(GL_QUADS);
        glColor3ubv(skymin);
        glVertex3f( -t, 0.0f,  t );
        glVertex3f(  t, 0.0f,  t );
        glColor3ubv(skymax);
        glVertex3f(  t,  t,  t );
        glVertex3f( -t,  t,  t );
    glEnd();

    // Render the right quad
    glBegin(GL_QUADS);
        glColor3ubv(skymin);
        glVertex3f( -t, 0.0f, -t );
        glVertex3f( -t, 0.0f,  t );
        glColor3ubv(skymax);
        glVertex3f( -t,  t,  t );
        glVertex3f( -t,  t, -t );
    glEnd();

    // Render the top quad
    glBegin(GL_QUADS);
        glColor3ubv(skymin);
        glVertex3f( -t,  t, -t );
        glVertex3f( -t,  t,  t );
        glColor3ubv(skymax);
        glVertex3f(  t,  t,  t );
        glVertex3f(  t,  t, -t );
    glEnd();

    glEnable(GL_LIGHTING);

/*
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_DEPTH);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint texture1;
    Texture::loadTexture(QString("cloud.jpg"), &texture1);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glRotatef(-90.0,1.0,0.0,0.0);

    GLUquadric* quadratic = gluNewQuadric();
    gluQuadricDrawStyle(quadratic, GLU_FILL);
    gluQuadricTexture(quadratic, GL_TRUE);

    gluSphere( quadratic, 100.0f, 32, 32);

    glEnable(GL_DEPTH);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glPopAttrib();
*/
}

/* Méthode pour récupérer les évènements de la molette de la souris */
void GLViewer::wheelEvent(QWheelEvent* event)
{
    Vec cameraPos0 = camera()->position();
    const float wheelSensitivityCoef = 8E-4f;
    float coef = qMax(fabsf((camera()->frame()->coordinatesOf(camera()->revolveAroundPoint())).z), 0.2f*camera()->sceneRadius());

    Vec cameraPos1(cameraPos0[0], cameraPos0[1], cameraPos0[2] + (coef * event->delta() * camera()->frame()->wheelSensitivity() * wheelSensitivityCoef));
    cameraPos1 = camera()->frame()->inverseTransformOf(cameraPos1);

    GLfloat pos1[4] = {cameraPos1[0], cameraPos1[1], cameraPos1[2], 1.0};

    const float d1 = sqrt(pos1[0]*pos1[0]+pos1[1]*pos1[1]+pos1[2]*pos1[2]);

    if (d1 > 2.0f && d1 < 50.0f)
        QGLViewer::wheelEvent(event);
}

/* Méthode pour initialiser la lumière */
void GLViewer::initLighting()
{
    GLfloat lum_diffuse [] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat lum_speculaire [] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat lum_ambiante [] = { 0.2, 0.2, 0.2, 1.0 };

    glEnable(GL_LIGHTING);

    // LIGHT0
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lum_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lum_speculaire);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lum_ambiante);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0);

    //glLightf(GL_LIGHT0, GL_SPOT_EXPONENT,  1.0);
    //glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.5);
    //glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.01);
    //glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 1.5);

    // LIGHT1
    //glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lum_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lum_speculaire);
    glLightfv(GL_LIGHT1, GL_AMBIENT, lum_ambiante);
    //glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,  3.0);
    //glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.5);
    //glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 1.0);
    //glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 1.5);
}


void GLViewer::setObjectMoving(bool b)
{
    objectMoving = b;
}


void GLViewer::keyPressEvent(QKeyEvent* event)
{
    if (mode == MODE_3D)
    {
        if ((event->key() == Qt::Key_R) && objectMoving)
            emit objectChangeRotation();

        if ((event->key() == Qt::Key_Delete) && objectMoving)
            emit deleteObject();
    }

    QGLViewer::keyPressEvent(event);
}

/* Méthode qui renvoie l'aide de l'application */
QString GLViewer::helpString() const
{
    QString text("<h2>Home Creator 3D</h2>");

    if (mode == MODE_2D)
    {
        text += "Tracer les contours de la maison<br>";
        text += "Utiliser le clique gauche pour modifier les points<br>";
        text += "Utiliser le clique droit pour se déplacer dans le plan 2D<br>";
        text += "Utiliser la molette pour effectuer un zoom<br>";
        text += "Pour visualiser la maison en 3D, cliquez sur le bouton Vue 3D.<br>";
    }
    else if (mode == MODE_3D)
    {
        text += "Utiliser le menu à droite pour sélectionner les textures ou les objets à placer dans la maison<br>";
        text += "Appuyer sur R pour faire pivoter de 90° un objet sélectionné ou Suppr pour le supprimer<br>";
        text += "Utiliser le clique droit pour tourner autour de la maison<br>";
        text += "Utiliser la molette pour effectuer un zoom<br>";
        text += "Il est possible d'enregistrer un projet. Pour cela, cliquez sur Fichier puis Enregistrer.<br>";
    }

    return text;
}

// EOF
