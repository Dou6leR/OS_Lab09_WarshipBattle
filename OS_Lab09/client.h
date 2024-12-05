#ifndef CLIENT_H
#define CLIENT_H

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <QString>
#include "exception.h"

class Client{
public:
    Client(const QString& path);
    ~Client();
    void create_socket();
    void connect_to_server();
    void send_data(const QString& data);
    QString receive_data();
    void close_connection();
    int get_socket();
private:
    int client_socket;
    QString socket_path;
    struct sockaddr_un server_address;
};


#endif // CLIENT_H
