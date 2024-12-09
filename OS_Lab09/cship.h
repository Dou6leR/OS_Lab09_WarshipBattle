#ifndef CSHIP_H
#define CSHIP_H
#include <QGraphicsScene>
#include <QMessageBox>
#include <QString>
#include "ccell.h"
class CShip : public QObject
{
    Q_OBJECT
public:
    //CShip();
    CShip(QGraphicsScene* in_scene, int PosX, int PosY,  QObject *parent = 0);

    void SwitchGridForShip();

    bool checkAllConection();

    void getAllShipPositions();
signals:
    void ShipPositions(QString positions);
private:
    QGraphicsScene* scene;
    QVector<CCell*> ships;
};

#endif // CSHIP_H
