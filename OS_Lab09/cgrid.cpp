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
        shipField[i]->setCursor(QCursor(Qt::CrossCursor));
    }
}
void CGrid::startRecievingShoots()
{
    isServerAnswered = true;
}

void CGrid::setDotsAroundKill(int x, int y, int size, bool isVertical)
{
    if(isVertical)
    {
        if(x != 0)
        {
            for(int i = 0; i < size; i++)
                if(shipField[x + (y + i) * 10 - 1]->_typeShip == 0)
                    shipField[x + (y + i) * 10 - 1]->changeType(_dot);

            if(y != 0)
                if(shipField[x + y * 10 - 11]->_typeShip == 0)
                    shipField[x + y * 10 - 11]->changeType(_dot);

            if(y + size - 1 != 9)
                if(shipField[x + y * 10 + 10 * size - 1]->_typeShip == 0)
                    shipField[x + y * 10 + 10 * size - 1]->changeType(_dot);
        }
        if(x != 9)
        {
            for(int i = 0; i < size; i++)
                if(shipField[x + (y + i) * 10 + 1]->_typeShip == 0)
                    shipField[x + (y + i) * 10 + 1]->changeType(_dot);

            if(y != 0)
                if(shipField[x + y * 10 - 9]->_typeShip == 0)
                    shipField[x + y * 10 - 9]->changeType(_dot);

            if(y + size - 1 != 9)
                if(shipField[x + y * 10 + 10 * size + 1]->_typeShip == 0)
                    shipField[x + y * 10 + 10 * size + 1]->changeType(_dot);
        }
        if(y != 0)
            if(shipField[x + y * 10 - 10]->_typeShip == 0)
                shipField[x + y * 10 - 10]->changeType(_dot);

        if(y + size - 1 != 9)
            if(shipField[x + y * 10 + 10 * size]->_typeShip == 0)
                shipField[x + y * 10 + 10 * size]->changeType(_dot);
    }
    else
    {
        if(y != 0)
        {
            for(int i = 0; i < size; i++)
                if(shipField[x + i + y * 10 - 10]->_typeShip == 0)
                    shipField[x + i + y * 10 - 10]->changeType(_dot);

            if(x != 0)
                if(shipField[x + y * 10 - 11]->_typeShip == 0)
                    shipField[x + y * 10 - 11]->changeType(_dot);

            if(x + size - 1 != 9)
                if(shipField[x + y * 10 - 10 + size]->_typeShip == 0)
                    shipField[x + y * 10 - 10 + size]->changeType(_dot);
        }
        if(y != 9)
        {
            for(int i = 0; i < size; i++)
                if(shipField[x + i + y * 10 + 10]->_typeShip == 0)
                    shipField[x + i + y * 10 + 10]->changeType(_dot);

            if(x != 0)
                if(shipField[x + y * 10 + 9]->_typeShip == 0)
                    shipField[x + y * 10 + 9]->changeType(_dot);

            if(x + size - 1 != 9)
                if(shipField[x + y * 10 + 10 + size]->_typeShip == 0)
                    shipField[x + y * 10 + 10 + size]->changeType(_dot);
        }
        if(x != 0)
            if(shipField[x + y * 10 - 1]->_typeShip == 0)
                shipField[x + y * 10 - 1]->changeType(_dot);

        if(x + size - 1 != 9)
            if(shipField[x + y * 10 + size]->_typeShip == 0)
                shipField[x + y * 10 + size]->changeType(_dot);
    }
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

void CGrid::recieveHitMissAttacker(int n, int type)
{
    shipField[n]->changeType(type);
    isServerAnswered = true;
}

void CGrid::receiveHitMissDefender(int n, int type)
{
    if(type == _dot)
        shipField[n]->changeType(type);
    else
    {
        killOnTop.push_back(new CCell(_kill, 0));
        killOnTop.last()->setPos(CCell::SIZE * (n % 10), 3 * CCell::SIZE + CCell::SIZE * (n / 10));
        scene->addItem(killOnTop.last());
    }
}

void CGrid::recieveKillAttacker(int size, int *ship)
{
    bool isVertical = false;
    int x = ship[0] % 10;
    int y = ship[0] / 10;
    if(size > 1)
        if((ship[1] - ship[0]) == 10)
            isVertical = true;


    newShips.push_back(new CCell(size, 0));
    newShips.last()->setPos(CCell::SIZE * (14 + x), CCell::SIZE * (3 + y));
    scene->addItem(newShips.last());
    setDotsAroundKill(x, y, size, isVertical);
    isServerAnswered = true;
}

void CGrid::recieveKillDefender(int size, int *ship)
{
    bool isVertical = false;
    int x = ship[0] % 10;
    int y = ship[0] / 10;
    if(size > 1)
        if((ship[1] - ship[0]) == 10)
            isVertical = true;
    setDotsAroundKill(x, y, size, isVertical);
}
