#ifndef CSHIP_H
#define CSHIP_H
#include <QGraphicsScene>
#include <QMessageBox>
#include "ccell.h"
class CShip
{
public:
    CShip();
    CShip(QGraphicsScene* in_scene, int PosX, int PosY);

    void SwitchGridForShip();

    bool checkAllConection();
private:
    QGraphicsScene* scene;
    QVector<CCell*> ships;
};

#endif // CSHIP_H