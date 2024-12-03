#include "ccell.h"


//#include "mainwindow.h"
QPixmap* CCell::dot;
QPixmap* CCell::cell;
QPixmap* CCell::hit;
QPixmap* CCell::kill;
QPixmap* CCell::sh_1;
QPixmap* CCell::sh_2;
QPixmap* CCell::sh_3;
QPixmap* CCell::sh_4;

CCell::CCell(int typeShip, QObject *parent) : QObject(parent), QGraphicsItem()
{
    _typeShip = typeShip;
    if(_typeShip != 0)
    {
        setFlag(QGraphicsItem::GraphicsItemFlag::ItemIsMovable); //Make ships movable
    }
    switch (_typeShip)
    {
    case _cell:
        ship = cell;
        _width = SIZE;
        _height = SIZE;
        break;

    case _sh_1:
        ship = sh_1;
        _width = SIZE;
        _height = SIZE;
        break;

    case _sh_2:
        ship = sh_2;
        _width = SIZE * 2;
        _height = SIZE;
        break;

    case _sh_3:
        ship = sh_3;
        _width = SIZE * 3;
        _height = SIZE;
        break;

    case _sh_4:
        ship = sh_4;
        _width = SIZE * 4;
        _height = SIZE;
        break;

    default:
        break;
    }
}

CCell::~CCell() {}

QRectF CCell::boundingRect() const
{
    return QRectF(0,0,_width,_height);
}

void CCell::initPixmap()
{
    //TO DO:
    //Intizlize static types "dot = new QPixmap("/path/.png");"

}

void CCell::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
    if(2 <= _typeShip && _typeShip <= 4) // rotate only ships(2,3,4)
    {
        if (isVertical==false)
        {
            setRotation(90);
            isVertical = true;
        }
        else
        {
            setRotation(0);
            isVertical = false;
        }
        QGraphicsItem::mousePressEvent(event);
    }
}

void CCell::changeType(int typeShip)
{
    _typeShip = typeShip;

    switch (_typeShip)
    {
    case _cell:
        ship = cell;
        _width = SIZE;
        _height = SIZE;
        break;

    case _sh_1:
        ship = sh_1;
        _width = SIZE;
        _height = SIZE;
        break;

    case _sh_2:
        ship = sh_2;
        _width = SIZE * 2;
        _height = SIZE;
        break;

    case _sh_3:
        ship = sh_3;
        _width = SIZE * 3;
        _height = SIZE;
        break;

    case _sh_4:
        ship  = sh_4;
        _width = SIZE * 4;
        _height = SIZE;
        break;

    case _dot:
        ship = dot;
        _width = SIZE;
        _height = SIZE;
        break;

    case _hit:
        ship = hit;
        _width = SIZE;
        _height = SIZE;
        break;

    case _kill:
        ship = kill;
        _width = SIZE;
        _height = SIZE;
        break;

    default:
        break;
    }
    update();
}

