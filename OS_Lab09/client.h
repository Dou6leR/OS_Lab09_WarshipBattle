#ifndef CLIENT_H
#define CLIENT_H

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string>

class Client{
public:
    Client(const std::string& path);
    ~Client();
    bool create_socket();
    bool connect_to_server();
    bool send_data(const std::string& data);
    std::string receive_data();
    void close_connection();
    int get_socket();
private:
    int client_socket;
    std::string socket_path;
    struct sockaddr_un server_address;
};


#endif // CLIENT_H
