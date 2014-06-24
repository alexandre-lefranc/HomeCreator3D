/*
 *  Description : Home Creator 3D
 *
 *  Auteur : Alexandre Lefranc, ÉTS (C) 2012
 */

#include "myconstraint.h"
#include <QGLViewer/camera.h>

using namespace qglviewer;

// Contrainte de rotation de la caméra
void MyConstraint::constrainRotation(Quaternion &rotation, Frame *const frame)
{
    Vec axis = rotation.axis();
    double angle = rotation.angle();

    //printf("[%lf %lf] \n", frame->orientation().angle(), rotation.angle());
    //printf("/%lf %lf %lf\\\n", frame->orientation().axis().x, frame->orientation().axis().y, frame->orientation().axis().z) ;
    // printf("<%lf %lf %lf>\n", frame->position().x, frame->position().y, frame->position().z);
    //printf("|%lf %lf %lf|\n", rotation.axis().x, rotation.axis().y, rotation.axis().z);
    //axis.z = 0;

    /*
    if ((frame->orientation().angle() + angle) > 3.14/2.0)
    {
        rotation.setAxisAngle(rotation.axis(), -rotation.angle());
        //frame->rotate(Quaternion(frame->orientation().axis(),0.0));
    }
    */
    /*
    Vec quat = Vec(rotation[0], rotation[1], rotation[3]);
    quat.projectOnAxis(axis);

    if (frame->position().y <= 0.3)
    angle = -angle;

    rotation = Quaternion(quat, angle);
    */

    Vec axisC (0.0,1.0,0.0);

    axis = frame->transformOf(camera()->frame()->inverseTransformOf(axisC));
    Vec quat = Vec(rotation[0], rotation[1], rotation[2]);
    quat.projectOnAxis(axis);
    rotation = Quaternion(quat, 2.0*acos(rotation[3]));
    // printf("|%lf %lf %lf|\n", rotation.axis().x, rotation.axis().y, rotation.axis().z);
}

// EOF
