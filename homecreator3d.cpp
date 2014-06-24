/*
 *  Description : Home Creator 3D
 *
 *  Auteur : Alexandre Lefranc, ÉTS (C) 2012
 */

#include <QDesktopWidget>
#include <QApplication>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>

#include "HomeCreator3D.h"
#include "ui_mainwindow.h"

/* Constructeur */
HomeCreator3D::HomeCreator3D()
{
    createConnection();
    maison = new Maison();
    window.getUi()->widget->setMaison(maison);
    textureSelected = NULL;
}

/* Affichage de l'application */
void HomeCreator3D::show()
{
    // QFile file(".qglviewer.xml");
    // file.remove();

    window.show();
    loadTextures();
    loadModels();
    maison->addTexture(&textures["moquette1.jpg"], FLOOR_NAME);

    //GLViewer v(NULL);
    //v.show();
    //v.resize(1024, 800);

    QDesktopWidget *widget = QApplication::desktop();

    int x = widget->width() / 2 - window.width() / 2;
    int y = widget->height() / 2 - window.height() / 2 - 25;

    window.move(QPoint(x, y));
}

/* Méthode qui permet de créer les connexions entre les différents objets */
void HomeCreator3D::createConnection()
{
    QObject::connect(&window, SIGNAL(itemSelected(QString)), this, SLOT(itemSelected(QString)));
    QObject::connect(window.getUi()->widget, SIGNAL(constructionOK()), this, SLOT(enable3D()));
    QObject::connect(window.getUi()->widget, SIGNAL(objectChangePosition(double, double, double)), this, SLOT(objectMove(double, double, double)));
    QObject::connect(window.getUi()->widget, SIGNAL(objectChangeRotation()), this, SLOT(objectRotate()));
    QObject::connect(window.getUi()->widget, SIGNAL(selection(int)), this, SLOT(selection(int)));
    QObject::connect(window.getUi()->widget, SIGNAL(deleteObject()), this, SLOT(deleteObject()));

    QObject::connect(&window, SIGNAL(newHouse()), this, SLOT(reInit()));
    QObject::connect(&window, SIGNAL(loadHouse(QString)), this, SLOT(loadMaison(QString)));
    QObject::connect(&window, SIGNAL(saveHouse(QString)), this, SLOT(saveMaison(QString)));
}

/* Méthode qui permet de charger les texutres */
void HomeCreator3D::loadTextures()
{
    QStringList listFilter;
    listFilter << "*.jpg";
    listFilter << "*.jpeg";

    QTreeWidgetItem* txts = new QTreeWidgetItem();
    txts->setText(0, "Textures");

    QTreeWidgetItem* sols = new QTreeWidgetItem();
    sols->setText(0, "Sols");

    QTreeWidgetItem* ext = new QTreeWidgetItem();
    ext->setText(0, "Murs extérieurs");

    QTreeWidgetItem* inte = new QTreeWidgetItem();
    inte->setText(0, "Murs intérieurs");

    QDirIterator it("textures/floor", listFilter, QDir::Files | QDir::NoSymLinks);
    while (it.hasNext())
    {
        QString pathFileName = it.next();
        QString fileName = pathFileName.right(pathFileName.length()-pathFileName.lastIndexOf("/")-1);
        textures.insert(fileName, Texture(pathFileName));

        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setText(0, fileName);
        item->setIcon(0, QIcon(pathFileName));
        sols->addChild(item);
    }


    QDirIterator it1("textures/wall/exterieur", listFilter, QDir::Files | QDir::NoSymLinks);
    while (it1.hasNext())
    {
        QString pathFileName = it1.next();
        QString fileName = pathFileName.right(pathFileName.length()-pathFileName.lastIndexOf("/")-1);
        textures.insert(fileName, Texture(pathFileName));

        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setText(0, fileName);
        item->setIcon(0, QIcon(pathFileName));
        ext->addChild(item);
    }


    QDirIterator it2("textures/wall/interieur", listFilter, QDir::Files | QDir::NoSymLinks);
    while (it2.hasNext())
    {
        QString pathFileName = it2.next();
        QString fileName = pathFileName.right(pathFileName.length()-pathFileName.lastIndexOf("/")-1);
        textures.insert(fileName, Texture(pathFileName));

        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setText(0, fileName);
        item->setIcon(0, QIcon(pathFileName));
        inte->addChild(item);
    }


    txts->addChild(sols);
    txts->addChild(ext);
    txts->addChild(inte);


    window.getUi()->treeWidget->insertTopLevelItem(0, txts);
}

/* Méthode qui charge les modèles OBJ */
void HomeCreator3D::loadModels()
{
    models.insert("Chair", new Model("models/chair.obj", 0.0005,0.0));
    models.insert("Chair2", new Model("models/chair2.obj", 0.0013,0.0));
    models.insert("Table", new Model("models/table.obj", 1.6,0.0));
    models.insert("Bookcase", new Model("models/bookcase.obj", 0.001,0.0));
    models.insert("Piano", new Model("models/piano.obj", 1.0,0.0));

    models.insert("Soccerball", new Model("models/soccerball.obj", 0.03,4.5));
    models.insert("Flowers", new Model("models/flowers.obj", 0.06,12.0));

    QTreeWidgetItem* objects = new QTreeWidgetItem();
    objects->setText(0, "Objets");

    QList<QString> keys(models.keys());

    for (int i = 0; i < keys.size(); ++i)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setText(0, keys[i]);
        objects->addChild(item);
    }

    window.getUi()->treeWidget->insertTopLevelItem(1, objects);
}

/* Méthode qui permet de récupérer le nom de l'item sélectionné
  dans le menu */
void HomeCreator3D::itemSelected(QString name)
{
    if (models.contains(name)) // Modèle
    {
        objectSelected = maison->addMobilier(models[name]);
        window.refreshRender();
        textureSelected = NULL;
    }    
    else if (textures.contains(name)) // Texture
    {
        textureSelected = &textures[name];
        objectSelected = NULL;
    }
}

/* Méthode qui permet de récupérer la position d'un objet en mouvement */
void HomeCreator3D::objectMove(double x, double y, double z)
{
    if (objectSelected != NULL)
        objectSelected->setPosition(x, 0.05, z);
}

/* Méthode qui permet d'activer le bouton Vue */
void HomeCreator3D::enable3D()
{
    window.enable3D();
}

/* Méthode qui permet de connaître si un objet est en rotation */
void HomeCreator3D::objectRotate()
{
    objectSelected->changeRotation();
    window.refreshRender();
}

/* Méthode qui permet de récupérer le nom de l'objet qui a été sélectionné */
void HomeCreator3D::selection(int name)
{
    if (textureSelected != NULL && maison->addTexture(textureSelected, name))
    {
        window.refreshRender();
    }
    else
    {
        if ((objectSelected = maison->findMobilier(name)) != NULL)
            window.getUi()->widget->setObjectMoving(true);
    }
}

/* Méthode qui permet de connaitre si un objet doit être supprimer */
void HomeCreator3D::deleteObject()
{
    if (objectSelected != NULL)
    {
        maison->deleteObject(objectSelected->getName());
        window.refreshRender();
    }
}

/* Méthode qui permet de réinitialiser les paramètres par défaut */
void HomeCreator3D::reInit()
{
    delete maison;
    maison = new Maison();
    maison->addTexture(&textures["moquette1.jpg"], FLOOR_NAME);
}

/* Méthode qui permet de charger une maison */
bool HomeCreator3D::loadMaison(QString fileName)
{
    reInit();

    if (maison->loadMaison(fileName, models, textures))
    {
        enable3D();
        window.getUi()->widget->reInit();
        window.refreshRender();
        return true;
    }
    return false;
}

/* Méthode qui permet de sauvegarder une maison */
bool HomeCreator3D::saveMaison(QString fileName)
{
    return maison->saveMaison(fileName);
}

// EOF
