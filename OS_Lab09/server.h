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
#include "my_message.h"

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
    QVector<QMap<int,bool>> client1_ships;
    QVector<QMap<int,bool>> client2_ships;
public slots:
    void process_server();
signals:
    void finished();
    void error(QString err);

    void log_signal(QString log);
private:
    bool check_killed(QMap<int,bool> ship);
    Server* m_server;
    QVector<int> client_sockets;
};

#endif // SERVER_H
