/*
 *  Description : Home Creator 3D
 *
 *  Auteur : Alexandre Lefranc, ÉTS (C) 2012
 */

#include "texture.h"

/* Constructeur de la classe */
Texture::Texture(QString fileName)
{
    _fileName = fileName.right(fileName.length()-fileName.lastIndexOf("/")-1);
    _idTexture = loadTexture(fileName);
}


/* Méthode qui permet de lier une texture */
void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, _idTexture);
}


/* Méthode qui permet de charger une texture */
GLuint Texture::loadTexture(QString textureName)
{
    QImage img, tmp;
    GLuint texture;

    if (!tmp.load(textureName))
    {
        printf("Impossible de charger la texture %s\n", textureName.toStdString().c_str());
        return -1;
    }
    img = QGLWidget::convertToGLFormat(tmp);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, img.width(), img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.bits());

    return texture;
}

// EOF
