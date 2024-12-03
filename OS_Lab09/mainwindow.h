#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "ccell.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    //Current scene
    QGraphicsScene  *scene;

    //All visible CCell items
    QVector<CCell*> shipField1;
    QVector<CCell*> shipField2;
    QVector<CCell*> ships;
};
#endif // MAINWINDOW_H
