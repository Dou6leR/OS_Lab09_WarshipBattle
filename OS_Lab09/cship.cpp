#include "cship.h"

CShip::CShip(QGraphicsScene* in_scene, int PosX, int PosY, QObject *parent) : QObject(parent)
{
    scene = in_scene;
    ships.resize(10);
    int i = 0, j = 1;

    for(int k = 1; k <= j; k++, i++)
    {
        ships[i] = new CCell(_sh_4, 1);
        ships[i]->setPos(PosX * (_sh_4 * (k - 1) + k), PosY * (10 - j * 2));
        scene->addItem(ships[i]);
    }
    j++;
    for(int k = 1; k <= j; k++, i++)
    {
        ships[i] = new CCell(_sh_3, 1);
        ships[i]->setPos(PosX * (_sh_3 * (k - 1) + k), PosY * (10 - j * 2));
        scene->addItem(ships[i]);
    }
    j++;
    for(int k = 1; k <= j; k++, i++)
    {
        ships[i] = new CCell(_sh_2, 1);
        ships[i]->setPos(PosX * (_sh_2 * (k - 1) + k), PosY * (10 - j * 2));
        scene->addItem(ships[i]);
    }
    j++;
    for(int k = 1; k <= j; k++, i++)
    {
        ships[i] = new CCell(_sh_1, 1);
        ships[i]->setPos(PosX * (_sh_1 * (k - 1) + k), PosX * (10 - j * 2));
        scene->addItem(ships[i]);
    }
}

void CShip::SwitchGridForShip()
{
    for(int i = 0; i < SHIPSNUM; i++)
        {
            ships[i]->deleteMovableOption();
            ships[i]->moveBy(-13 * CCell::SIZE, 0);
        }
}

bool CShip::checkAllConection()
{
    for(int i = 0; i < SHIPSNUM;  i++)
    {
        if(!(ships[i]->isConToTable))
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Attention!");
            msgBox.setText("Place all ships first and check if all ships in connected to the grid");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Information);
            msgBox.exec();
            return true;
        }
    }
    return false;
}

void CShip::getAllShipPositions()
{
    QString positions = "";
    for(int i = 0; i < SHIPSNUM; i++)
    {
        for(int j = 0; j < ships[i]->_width / CCell::SIZE; j++)
            positions += QString::number(ships[i]->PositionOfShip[j]) + ",";
        positions.removeAt(positions.size() - 1);
        positions += " ";
    }
    positions.removeAt(positions.size() - 1);
    emit ShipPositions(positions);
}

void CShip::randomShipsPositions()
{
    int pos = -1;
    bool isVertical = false;
    for(int currentship = 0; currentship < SHIPSNUM; currentship++)
    {
        if(ships[currentship]->isConToTable)
        {
            ships[currentship]->isConToTable = false;
            for(int i = 0; i < ships[currentship]->_typeShip; i++)
                ships[currentship]->ShipTable[ships[currentship]->PositionOfShip[i]] = false;
            delete[] ships[currentship]->PositionOfShip;
        }
    }

    for(int currentship = 0; currentship < SHIPSNUM; currentship++)
    {
        int size = ships[currentship]->_typeShip;
        while(true) // Break only if ship fits
        {
            pos = QRandomGenerator::global()->bounded(100);
            int x = pos % 10;
            int y = pos / 10;
            isVertical  = QRandomGenerator::global()->bounded(2);
            if(isVertical)
            {
                if(y + size > 10)
                    continue;
            }
            else
            {
                if(x + size > 10)
                    continue;
            }
            if(ships[currentship]->isVertical != isVertical)
                ships[currentship]->rotateShip();

            if(ships[currentship]->isConflicted(x, y))
                continue;
            else
            {
                ships[currentship]->PositionOfShip = new int[size];

                for(int i = 0; i < size; i++)
                {
                    ships[currentship]->PositionOfShip[i] = x + y * 10 + i * (isVertical ? 10 : 1);
                    ships[currentship]->ShipTable[ships[currentship]->PositionOfShip[i]] = true;
                }
                ships[currentship]->isConToTable = true;
                ships[currentship]->setPos(CCell::SIZE * (14 + x), CCell::SIZE * (3 + y));
                break;
            }

        }
    }
}
