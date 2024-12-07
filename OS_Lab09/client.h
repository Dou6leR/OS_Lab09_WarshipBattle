#ifndef CLIENT_H
#define CLIENT_H

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <QString>
#include <QObject>
#include <QThread>
#include "exception.h"

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
public slots:
    void process_client();
signals:
    void finished();
    void error(QString err);
private:
    Client* m_client;
};


#endif // CLIENT_H
