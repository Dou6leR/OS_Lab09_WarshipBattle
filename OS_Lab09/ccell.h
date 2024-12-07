#ifndef CCELL_H
#define CCELL_H
#include <QObject>
#include <QGraphicsItem>
#include <QPainter>

//Enumerator of cell types
enum CellTypes
{
    _cell,
    _sh_1,
    _sh_2,
    _sh_3,
    _sh_4,
    _dot,
    _hit,
    _kill
};


//Class to operate
class CCell : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    //Const size of every cell
    static const int SIZE = 28;

    //Constructor
    explicit CCell(int typeShip, QObject *parent = 0);
    ~CCell();


    //Fiels
    int _width;
    int _height;
    int _typeShip;
    bool isVertical=false;
    QPixmap* cellData;


    //All static types of Pixmap(items)
    static QPixmap* cell;
    static QPixmap* sh_1;
    static QPixmap* sh_2;
    static QPixmap* sh_3;
    static QPixmap* sh_4;
    static QPixmap* dot;
    static QPixmap* hit;
    static QPixmap* kill;

    //Initialize types of Pixmap
    static void initPixmap();

    //Change type of cell
    void changeType(int type);

protected:
    //Rotate the ship
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);

private:
    //Return coordinates
    QRectF boundingRect() const;

    //Paint the cell with (QPixmap *celldata)
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // CCELL_H
