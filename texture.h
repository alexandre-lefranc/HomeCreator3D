/*
 *  Description : Home Creator 3D
 *
 *  Auteur : Alexandre Lefranc, ÉTS (C) 2012
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include <QString>
#include <QtOpenGL>

// Classe Texture
class Texture
{
private:
    GLuint _idTexture;
    QString _fileName;

public:
    Texture() {}
    Texture(QString fileName);

    GLuint getTextureId() { return _idTexture; }
    QString getFileName() { return _fileName; }
    void bind();

    GLuint static loadTexture(QString textureName);
};

#endif // TEXTURE_H
