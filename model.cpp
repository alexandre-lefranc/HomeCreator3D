/*
 *  Description : Home Creator 3D
 *
 *  Auteur : Alexandre Lefranc, ÉTS (C) 2012
 */

#include "model.h"

/* Constructeur */
Model::Model(QString fileName, double scaleValue, double YtranslationValue)
{
    _scaleValue = scaleValue;
    _model = glmReadOBJ((char*)fileName.toStdString().c_str());
    _modelName = fileName.mid(fileName.lastIndexOf("/")+1,fileName.lastIndexOf(".")-fileName.lastIndexOf("/")-1);
    _modelName = _modelName[0].toUpper() + _modelName.right(_modelName.length()-1);
    _ty = YtranslationValue;
}

/* Destructeur */
Model::~Model()
{
    glmDelete(_model);
}

/* Méthode qui renvoie l'objet model */
GLMmodel* Model::getModel()
{
    return _model;
}

/* Méthode qui permet de dessiner le modèle */
void Model::draw()
{
    glPushMatrix();
        glEnable(GL_NORMALIZE);
        glShadeModel(GL_SMOOTH);

        glScaled(_scaleValue, _scaleValue, _scaleValue);
        glTranslated(0.0,_ty,0.0);

        glmDraw(_model, GLM_SMOOTH | GLM_MATERIAL);

    glPopMatrix();
}

// EOF
