#ifndef CCELL_H
#define CCELL_H
#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#define SHIPSNUM 10

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
    static const int SIZE = 30;

    //Constructor
    explicit CCell(int typeShip, bool IsMovable, QObject *parent = 0);
    ~CCell();

    //Delete the movable option for an onbject
    void deleteMovableOption();

    //Fiels
    int _width;
    int _height;
    int _typeShip;
    bool isVertical = false;
    bool isConToTable = false; // Check if ship connected to table
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

    void rotateShip();

    //Table to check ship conficts static to make shared between other ships
    static bool* ShipTable;
    //Position of current ship
    int* PositionOfShip = nullptr;

protected:
    //Rotate the ship
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    //Return coordinates
    QRectF boundingRect() const;

    //Check conflicts with other ships
    bool isConflicted(int x, int y);

    //Paint the cell with (QPixmap *celldata)
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
signals:
    void sendShootCoordinate(int n);
};

#endif // CCELL_H
