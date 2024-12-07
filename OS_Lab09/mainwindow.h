#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QVector>
#include <QGraphicsScene>
#include "ccell.h"
#include "server.h"
#include "client.h"
#include <QMainWindow>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>

//UI managing
#define W_WIDTH 800
#define W_HIGHT 600
enum MW_PAGES {
    WELCOME_PAGE,
    SHIP_PLACE_PAGE,
    SERVER_PAGE,
    GAME_PAGE,
};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void connect_client(Client* client);
    void connect_server();
    void send_to_server(Client* client);
    void accept_client();
    void receive_data(int id);
    void send_client1();
    void send_client2();
    void receive_from_server(Client* new_client);
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void switch_page(int page);

private slots:
    void on_server_but_clicked();

    void on_player_but_clicked();

    void on_ready_but_clicked();

private:
    Ui::MainWindow *ui;

    Server* server;
    QVector<int> client_sockets;
    
    //Current scene
    QGraphicsScene  *scene;

    //All visible CCell items
    QVector<CCell*> shipField1;
    QVector<CCell*> shipField2;
    QVector<CCell*> ships;

    //Add all ships and 2 grids
    void initShipsAndGrids();

};
#endif // MAINWINDOW_H
