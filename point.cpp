/*
 *  Description : Home Creator 3D
 *
 *  Auteur : Alexandre Lefranc, ÉTS (C) 2012
 */

#include "point.h"

GLuint Point::num = 0;
GLubyte Point::colorDefault[3] = {95,95,95};

// Constructeur
Point::Point(double x, double y) { _x = x; _y = y; _name = num++; setColor(colorDefault); }

// EOF
