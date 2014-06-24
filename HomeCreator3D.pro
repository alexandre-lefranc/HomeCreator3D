QT          += core gui opengl xml

TARGET      = HomeCreator3D
TEMPLATE    = app

INCLUDEPATH += inc

HEADERS += \
    glm.h \
    point.h \
    myconstraint.h \
    model.h \
    mobilier.h \
    mainwindow.h \
    glviewer.h \
    maison.h \
    texture.h \
    mur.h \
    ui_mainwindow.h \
    ui_aboutdialog.h \
    texture.h \
    point.h \
    myconstraint.h \
    mur.h \
    model.h \
    mobilier.h \
    maison.h \
    mainwindow.h \
    glviewer.h \
    glm.h \
    homecreator3d.h

SOURCES += \
    glm.cpp \
    point.cpp \
    myconstraint.cpp \
    model.cpp \
    mobilier.cpp \
    maison.cpp \
    mainwindow.cpp \
    main.cpp \
    glviewer.cpp \
    texture.cpp \
    mur.cpp \
    texture.cpp \
    point.cpp \
    myconstraint.cpp \
    mur.cpp \
    model.cpp \
    mobilier.cpp \
    maison.cpp \
    mainwindow.cpp \
    main.cpp \
    glviewer.cpp \
    glm.cpp \
    homecreator3d.cpp

INCLUDEPATH += C:\QtSDK\libQGLViewer-2.3.16
LIBS        += -LC:\QtSDK\libQGLViewer-2.3.16\QGLViewer-build-desktop-Qt_4_7_4_for_Desktop_-_MSVC2008__Qt_SDK__Release\release -lQGLViewer2

LIBS        += -LC:/QtSDK/Desktop/Qt/4.7.4/msvc2008/lib -lglut32

FORMS       += mainwindow.ui \
               aboutdialog.ui

RESOURCES   += \
    HomeCreator3D.qrc
