#include "cgrid.h"

CGrid::CGrid(QGraphicsScene* in_scene, int PosX, int PosY, QObject *parent) : QObject(parent)
{
    scene = in_scene;
    shipField.resize(100);
    for(int x = 0; x < 10; x++)
    {
        for(int y = 0; y < 10; y++)
        {
            shipField[x + y * 10] = new CCell(_cell, 0);
            connect(shipField[x + y * 10], &CCell::sendShootCoordinate, this, &CGrid::recieveClickedCell);
            shipField[x + y * 10]->setPos(PosX + x * CCell::SIZE, PosY + y * CCell::SIZE);
            scene->addItem(shipField[x + y * 10]);
        }
    }
}

void CGrid::hide()
{
    for(int i = 0; i < 100; i++)
        shipField[i]->hide();
}

void CGrid::show()
{
    for(int i = 0; i < 100; i++)
        shipField[i]->show();
}

void CGrid::SetCoolCursor()
{
    for (int i = 0; i < 100; i++)
    {
        //shipField[i]->setCursor(Qt::CrossCursor);
    }
}
void CGrid::startRecievingShoots()
{
    isServerAnswered = true;
}
void CGrid::recieveClickedCell(int n)
{
    if(isServerAnswered) // Don't change received cell until server answered than receive next cell
    {
        isServerAnswered = false;
        receivedCell = n;
        emit sendCellToServer(receivedCell);
        qDebug() << n;
    }

}
