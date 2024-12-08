#ifndef CGRID_H
#define CGRID_H
#include <QGraphicsScene>
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
    int receivedCell = -1;
    bool isServerAnswered = false;
    QGraphicsScene* scene;
    QVector<CCell*> shipField;

public slots:
    void recieveClickedCell(int n);
signals:
    void sendCellToServer(int n);
};

#endif // CGRID_H
