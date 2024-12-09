#ifndef CGRID_H
#define CGRID_H
#include <QGraphicsScene>
#include <QCursor>
#include "ccell.h"
class CGrid: public QObject
{
    Q_OBJECT
public:
    //CGrid();
    CGrid(QGraphicsScene* in_scene, int PosX, int PosY, QObject *parent = 0);

    void show();
    void hide();

    void SetCoolCursor();

    void startRecievingShoots();

private:
    void setDotsAroundKill(int x, int y, int size, bool isVertical);

    int receivedCell = -1;
    bool isServerAnswered = false;
    QGraphicsScene* scene;
    QVector<CCell*> shipField;
    QVector<CCell*> killOnTop;
    QVector<CCell*> newShips;

public slots:
    void recieveClickedCell(int n); // received from ccell

    void recieveHitMissAttacker(int n, int type); // For attacker to change second grid

    void receiveHitMissDefender(int n, int type); // For defender to change first grid

    void recieveKillAttacker(int size, int *ship); // For attacker to change second grid

    void recieveKillDefender(int size, int *ship); // For defender to change first grid
signals:
    void sendCellToServer(int n); // send further to client/server
};

#endif // CGRID_H
