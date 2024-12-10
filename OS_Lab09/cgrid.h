#ifndef CGRID_H
#define CGRID_H
#include <QGraphicsScene>
#include <QCursor>
#include <QLabel>
#include "ccell.h"

enum LabelText
{
    YOUR_TURN,
    ENEMY_TURN,
    YOU_WIN,
    YOU_LOSE
};

class CGrid: public QObject
{
    Q_OBJECT
public:
    //CGrid();
    CGrid(QGraphicsScene* in_scene, int PosX, int PosY, QLabel *turn_wan_label, QObject *parent = 0);

    void show();
    void hide();

    void SetCoolCursor();

    static bool *isMyTurn;

private:
    void setDotsAroundKill(int x, int y, int size, bool isVertical);

    void ChangeLabelText(int textType);

    int receivedCell = -1;

    QGraphicsScene* scene;
    QLabel *turnWinLabel;
    QVector<CCell*> shipField;
    QVector<CCell*> killOnTop;
    QVector<CCell*> newShips;

public slots:
    void recieveClickedCell(int n); // received from ccell

    void recieveMissAttacker(int n); // For attacker to change second grid

    void recieveHitAttacker(int n); // For attacker to change second grid

    void recieveMissDefender(int n); // For defender to change first grid

    void recieveHitDefender(int n); // For defender to change first grid

    void recieveKillAttacker(QVector<int> ship); // For attacker to change second grid

    void recieveKillDefender(QVector<int> ship); // For defender to change first grid

    void recieveWin();

    void recieveLose(QVector<QVector<QPair<int, bool>>> remainingShips); // to show all remaining ships

    void startRecievingShoots(bool turn);
signals:
    void sendCellToServer(int n); // send further to client/server
};

#endif // CGRID_H
