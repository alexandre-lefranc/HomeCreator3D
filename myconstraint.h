/*
 *  Description : Home Creator 3D
 *
 *  Auteur : Alexandre Lefranc, ÉTS (C) 2012
 */

#ifndef MYCONSTRAINT_H
#define MYCONSTRAINT_H

#include <QGLViewer/qglviewer.h>
#include <QGLViewer/constraint.h>
#include <QGLViewer/frame.h>
#include <QtOpenGL>

#include "math.h"

using namespace qglviewer;

// Classe MyConstraint
class MyConstraint : public Constraint, QGLViewer
{
public:
    virtual void constrainRotation(Quaternion &rotation, Frame *const frame);
};

#endif // MYCONSTRAINT_H
