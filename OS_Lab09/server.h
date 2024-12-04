#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string>

class Server {
public:
    Server(const std::string& path);
    ~Server();
    bool create_socket();
    bool bind_socket();
    bool listen_for_clients(int backlog = 5);
    int accept_client();
    bool send_data(int client_socket, const std::string& data);
    std::string receive_data(int client_socket);
    void close_server();
private:
    int server_socket;
    struct sockaddr_un server_address;
    std::string socket_path;
};

#endif // SERVER_H
