#ifndef CLIENT_H
#define CLIENT_H

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <QList>
#include <QString>
#include <QObject>
#include <QThread>
#include "exception.h"
#include "my_message.h"

class Client : public QObject
{
    Q_OBJECT
public:
    Client(const QString& path);
    ~Client();
    void create_socket();
    void connect_to_server();
    void send_data(const QString& data);
    QString receive_data();
    void close_connection();
    void client_init();
signals:
    void wait_unlock();
public slots:
    void send_shoot(int n);
    void send_ship_positions(QString positions);
private:
    int client_socket;
    QString socket_path;
    struct sockaddr_un server_address;
};

class ClientController : public QObject
{
    Q_OBJECT
public:
    explicit ClientController(Client* client);
    bool wait_for_msg = false;
public slots:
    void wait_unlock();
    void process_client();
signals:
    void to_shooter_hit_msg(int position);
    void to_shooter_miss_msg(int position);
    void to_receiver_hit_msg(int position);
    void to_receiver_miss_msg(int position);

    void shooter_kill_msg(QVector<int> ship);
    void receiver_kill_msg(QVector<int> ship);

    void ready_msg(bool turn);

    void win_lose_msg(bool win);  // 0 - lose, 1 - win

    void finished();
    void error(QString err);
private:
    Client* m_client;
};


#endif // CLIENT_H
