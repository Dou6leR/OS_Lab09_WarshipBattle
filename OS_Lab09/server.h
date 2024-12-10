#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <QString>
#include <QTextEdit>
#include "exception.h"
#include <QThread>
#include <QObject>
#include <QRandomGenerator>
#include <QVector>
#include "my_message.h"

class Fleet
{
public:
    Fleet();
    ~Fleet();
    // [0] element is ignored
    void set_fleet(QString source);
    //0 - miss 1 - hit 2 - kill
    int hit_if_exist(int coord);
    // if kill use this
    QString get_ship(int coord) const;
    // ouput for log
    QString output_fleet() const;
private:
    QVector<QVector<QPair<int, bool>>> ships;
    bool is_killed(QVector<QPair<int, bool>> ship) const;
};


class Server : public QObject{
    Q_OBJECT
public:
    Server(const QString& path, QTextEdit* _server_log);
    ~Server();
    void create_socket();
    void bind_socket();
    void listen_for_clients(int backlog = 5);
    int accept_client();
    void send_data(int client_socket, const QString& data);
    QString receive_data(int client_socket);
    void close_server();
    void server_init();
public slots:
    void put_in_log(QString log);
signals:
    void log_signal(QString log);
private:
    int server_socket;
    struct sockaddr_un server_address;
    QString socket_path;
    QTextEdit* server_log;
};

class ServerController : public QObject
{
    Q_OBJECT
public:
    explicit ServerController(Server* server);
    Fleet client1_ships;
    Fleet client2_ships;

    void send_message(int type, QString message, bool turn, QString message_prefix = "");
    QString form_ship_return();

public slots:
    void process_server();

signals:
    void finished();
    void error(QString err);

    void log_signal(QString log);
private:
    Server* m_server;
    QVector<int> client_sockets;
};

#endif // SERVER_H
