#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "server.h"
#include "client.h"

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

private slots:

    void on_player_btn_clicked();

    void on_server_btn_clicked();

private:
    Ui::MainWindow *ui;
    Server* server;
    QVector<int> client_sockets;
};
#endif // MAINWINDOW_H
