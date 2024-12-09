#ifndef CSHIP_H
#define CSHIP_H
#include <QGraphicsScene>
#include <QMessageBox>
#include <QString>
#include <QRandomGenerator>
#include "ccell.h"
class CShip : public QObject
{
    Q_OBJECT
public:
    //CShip();
    CShip(QGraphicsScene* in_scene, int PosX, int PosY, QObject *parent = 0);

    void SwitchGridForShip();

    bool checkAllConection();

    QString getAllShipPositions();

public slots:
    void randomShipsPositions();
private:
    QGraphicsScene* scene;
    QVector<CCell*> ships;
};

#endif // CSHIP_H
