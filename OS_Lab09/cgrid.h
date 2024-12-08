#ifndef CGRID_H
#define CGRID_H
#include <QGraphicsScene>
#include "ccell.h"
class CGrid
{
public:
    CGrid();
    CGrid(QGraphicsScene* in_scene, int PosX, int PosY);

    void show();
    void hide();

    void SetCoolCursor();

private:
    QGraphicsScene* scene;
    QVector<CCell*> shipField;
};

#endif // CGRID_H
