#include "cship.h"

CShip::CShip(QGraphicsScene* in_scene, int PosX, int PosY)
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

QString CShip::getAllShipPositions()
{
    QString positions = "";
    for(int i = 0; i < SHIPSNUM; i++)
    {
        for(int j = 0; j < ships[i]->_width / CCell::SIZE; j++)
            positions += QString::number(ships[i]->PositionOfShip[j] + " ";
        positions += ",";
    }
}
