/*
 *  Description : Home Creator 3D
 *
 *  Auteur : Alexandre Lefranc, ÉTS (C) 2012
 */

#ifndef MODEL_H
#define MODEL_H

#include <QString>
#include "glm.h"

// Classe Model
class Model
{
private:
    GLMmodel* _model;
    double _scaleValue, _ty;
    QString _modelName;

public:
    Model(QString fileName, double scaleValue, double YtranslationValue);
    ~Model();

    GLMmodel* getModel();
    QString getModelName() { return _modelName; }
    void draw();
};

#endif // MODEL_H
