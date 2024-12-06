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
        cellData = cell;
        _width = SIZE;
        _height = SIZE;
        break;

    case _sh_1:
        cellData = sh_1;
        _width = SIZE;
        _height = SIZE;
        break;

    case _sh_2:
        cellData = sh_2;
        _width = SIZE * 2;
        _height = SIZE;
        break;

    case _sh_3:
        cellData = sh_3;
        _width = SIZE * 3;
        _height = SIZE;
        break;

    case _sh_4:
        cellData = sh_4;
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
    dot = new QPixmap(":/images/dot.png");
    cell = new QPixmap(":/images/cell.png");
    kill = new QPixmap(":/images/kill.png");
    hit = new QPixmap(":/images/hit.png");
    sh_1 = new QPixmap(":/images/sh_1.png");
    sh_2 = new QPixmap(":/images/sh_2.png");
    sh_3 = new QPixmap(":/images/sh_3.png");
    sh_4 = new QPixmap(":/images/sh_4.png");

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
        Q_UNUSED(event);
    }
}

void CCell::changeType(int typeShip)
{
    _typeShip = typeShip;

    switch (_typeShip)
    {
    case _cell:
        cellData = cell;
        _width = SIZE;
        _height = SIZE;
        break;

    case _sh_1:
        cellData = sh_1;
        _width = SIZE;
        _height = SIZE;
        break;

    case _sh_2:
        cellData = sh_2;
        _width = SIZE * 2;
        _height = SIZE;
        break;

    case _sh_3:
        cellData = sh_3;
        _width = SIZE * 3;
        _height = SIZE;
        break;

    case _sh_4:
        cellData = sh_4;
        _width = SIZE * 4;
        _height = SIZE;
        break;

    case _dot:
        cellData = dot;
        _width = SIZE;
        _height = SIZE;
        break;

    case _hit:
        cellData = hit;
        _width = SIZE;
        _height = SIZE;
        break;

    case _kill:
        cellData = kill;
        _width = SIZE;
        _height = SIZE;
        break;

    default:
        break;
    }
    update();
}

void CCell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(0, 0, *cellData, 0, 0, _width, _height);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}
