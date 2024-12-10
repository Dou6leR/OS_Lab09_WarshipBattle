#ifndef CLIENT_H
#define CLIENT_H

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <QList>
#include <QString>
#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QtGlobal>
#include "exception.h"
#include "my_message.h"

extern QMutex mutex;
extern QWaitCondition condition;
extern bool wait_for_msg;

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

    void to_shooter_hit_msg(int position);
    void to_shooter_miss_msg(int position);
    void to_receiver_hit_msg(int position);
    void to_receiver_miss_msg(int position);

    void shooter_kill_msg(QVector<int> ship);
    void receiver_kill_msg(QVector<int> ship);

    void ready_msg(bool turn);

    void win_msg();
    void lose_msg(QVector<QVector<QPair<int,bool>>> remaining_ships);
public slots:
    void send_shoot(int n);
    void send_ship_positions(QString positions);

    void s_to_shooter_hit_msg(int position);
    void s_to_shooter_miss_msg(int position);
    void s_to_receiver_hit_msg(int position);
    void s_to_receiver_miss_msg(int position);

    void s_shooter_kill_msg(QVector<int> ship);
    void s_receiver_kill_msg(QVector<int> ship);

    void s_ready_msg(bool turn);

    void s_win_msg();
    void s_lose_msg(QVector<QVector<QPair<int,bool>>> remaining_ships);
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

    Client* m_client;

    QVector<QVector<QPair<int,bool>>> lose_message(QString);
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

    void win_msg();
    void lose_msg(QVector<QVector<QPair<int,bool>>> remaining_ships);

    void finished();
    void error(QString err);
private:

};


#endif // CLIENT_H
