#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <QString>
#include "exception.h"

class Server {
public:
    Server(const QString& path);
    ~Server();
    void create_socket();
    void bind_socket();
    void listen_for_clients(int backlog = 5);
    int accept_client();
    void send_data(int client_socket, const QString& data);
    QString receive_data(int client_socket);
    void close_server();
private:
    int server_socket;
    struct sockaddr_un server_address;
    QString socket_path;
};

#endif // SERVER_H
