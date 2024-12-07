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
bool* CCell::ShipTable = new bool[100]{0};


CCell::CCell(int typeShip, bool IsMovable, QObject *parent) : QObject(parent), QGraphicsItem()
{
    _typeShip = typeShip;
    if(_typeShip != 0 && IsMovable)
    {
        setFlag(QGraphicsItem::GraphicsItemFlag::ItemIsMovable); //Make ships movable
        setFlag(QGraphicsItem::GraphicsItemFlag::ItemSendsScenePositionChanges); // Update position to make connect to the table
        isVertical = false;
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

void CCell::deleteMovableOption()
{
    setFlag(QGraphicsItem::GraphicsItemFlag::ItemIsMovable, false);
    setFlag(QGraphicsItem::GraphicsItemFlag::ItemSendsScenePositionChanges, false);
}

QRectF CCell::boundingRect() const
{
    return QRectF(0, 0, _width, _height);
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

void CCell::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(flags() & QGraphicsItem::ItemIsMovable)
    {
        if (2 <= _typeShip && _typeShip <= 4) // Rotate only ship (2, 3, 4)
        {
            if (!isVertical)
            {
                setTransformOriginPoint(SIZE / 2, SIZE / 2); // set point of rotate in the midlle of top left
                setRotation(90);
                isVertical = true;
            }
            else
            {
                setTransformOriginPoint(SIZE / 2, SIZE / 2); // set point of rotate in the midlle of top left
                setRotation(0);
                isVertical = false;
            }
            Q_UNUSED(event);
        }
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

QVariant CCell::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene())
    {
        if(isConToTable)
        {
            isConToTable = false;
            for(int i = 0; i < _width / SIZE; i++)
                ShipTable[PositionOfShip[i]] = false;
        }
        // value is the new position.
        QPointF newPos = value.toPointF();
      
        if(newPos.x() > 750)
        {
            newPos.setX(750);
            return newPos;
        }
        if(newPos.x() < 0)
        {
            newPos.setX(0);
            return newPos;
        }
        if(newPos.y() > 400)
        {
            newPos.setY(400);
            return newPos;
        }
        if(newPos.y() < 10)
        {
            newPos.setY(10);
            return newPos;
        }
        int snappedX = (newPos.x() - (14 * SIZE)) / SIZE;
        int snappedY = (newPos.y() - (3 * SIZE)) / SIZE;

        // Check if ship is fully in the grid
        if(isVertical)
        {
            if(snappedY + _width / SIZE > 10)
                return newPos;
        }
        else
        {
            if(snappedX + _width / SIZE > 10)
                return newPos;
        }


        // Ensure the item stays within the bounds of the grid (10x10)
        if (snappedX >= 0 && snappedX < 10 && snappedY >= 0 && snappedY < 10)
        {
            if(isConflicted(snappedX, snappedY))
                return newPos;
            PositionOfShip = new int[(_width / SIZE)];

            for(int i = 0; i < _width / SIZE; i++)
            {
                PositionOfShip[i] = snappedX + snappedY * 10 + i * (isVertical ? 10 : 1);
                ShipTable[PositionOfShip[i]] = true;
            }
            isConToTable = true;
            //Snap to the nearest grid point
            newPos.setX(14 * SIZE + snappedX * SIZE);
            newPos.setY(3 * SIZE + snappedY * SIZE);
            return newPos;
        }
    }

    return QGraphicsItem::itemChange(change, value); // Call the base class method for other changes
}

bool CCell::isConflicted(int x, int y)
{
    int size = _width / SIZE;
    if(isVertical)
    {
        if(x != 0)
        {
            for(int i = 0; i < size; i++)
                if(ShipTable[x + (y + i) * 10 - 1]) return true;

            if(y != 0)
                if(ShipTable[x + y * 10 - 11]) return true;

            if(y + size - 1 != 9)
                if(ShipTable[x + y * 10 + 10 * size - 1]) return true;
        }
        if(x != 9)
        {
            for(int i = 0; i < size; i++)
                if(ShipTable[x + (y + i) * 10 + 1]) return true;

            if(y != 0)
                if(ShipTable[x + y * 10 - 9]) return true;

            if(y + size - 1 != 9)
                if(ShipTable[x + y * 10 + 10 * size + 1]) return true;
        }
        if(y != 0)
            if(ShipTable[x + y * 10 - 10]) return true;

        if(y + size - 1 != 9)
            if(ShipTable[x + y * 10 + 10 * size]) return true;

        for(int i = 0; i < size; i++)
            if(ShipTable[x + (y + i) * 10]) return true;
    }
    else
    {
        if(y != 0)
        {
            for(int i = 0; i < size; i++)
                if(ShipTable[x + i + y * 10 - 10]) return true;

            if(x != 0)
                if(ShipTable[x + y * 10 - 11]) return true;

            if(x + size - 1 != 9)
                if(ShipTable[x + y * 10 - 10 + size]) return true;
        }
        if(y != 9)
        {
            for(int i = 0; i < size; i++)
                if(ShipTable[x + i + y * 10 + 10]) return true;

            if(x != 0)
                if(ShipTable[x + y * 10 + 9]) return true;

            if(x + size - 1 != 9)
                if(ShipTable[x + y * 10 + 10 + size]) return true;
        }
        if(x != 0)
            if(ShipTable[x + y * 10 - 1]) return true;

        if(x + size - 1 != 9)
            if(ShipTable[x + y * 10 + size]) return true;

        for(int i = 0; i < size; i++)
            if(ShipTable[x + i + y * 10]) return true;
    }
    return false;
}
