#include "server.h"

Server::Server(const std::string& path) : socket_path(path), server_socket(-1) {}

Server::~Server() {
    close_server();
}

bool Server::create_socket() {
    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Failed to create socket");
        return false;
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sun_family = AF_UNIX;
    strncpy(server_address.sun_path, socket_path.c_str(), sizeof(server_address.sun_path) - 1);

    return true;
}

bool Server::bind_socket(){
    unlink(socket_path.c_str());

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Failed to bind socket");
        return false;
    }

    return true;
}

bool Server::listen_for_clients(int backlog) {
    if (listen(server_socket, backlog) == -1) {
        perror("Failed to listen on socket");
        return false;
    }
    return true;
}

int Server::accept_client() {
    int client_socket = accept(server_socket, nullptr, nullptr);
    if (client_socket == -1) {
        perror("Failed to accept client connection");
    }
    return client_socket;
}

bool Server::send_data(int client_socket, const std::string& data) {
    if (send(client_socket, data.c_str(), data.length(), 0) == -1) {
        perror("Failed to send data");
        return false;
    }
    return true;
}

std::string Server::receive_data(int client_socket) {
    char buffer[1024] = {0};
    int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received == -1) {
        perror("Failed to receive data");
        return "";
    }
    return std::string(buffer, bytes_received);
}

void Server::close_server() {
    if (server_socket != -1) {
        close(server_socket);
        unlink(socket_path.c_str());
        server_socket = -1;
    }
}
