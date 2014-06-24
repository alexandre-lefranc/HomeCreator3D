/*
 *  Description : Home Creator 3D
 *
 *  Auteur : Alexandre Lefranc, ÉTS (C) 2012
 */

#include <QtGui/QApplication>

#include "HomeCreator3D.h"

/* Fonction principale */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    HomeCreator3D app;

    app.show();

    return a.exec();
}

// EOF
