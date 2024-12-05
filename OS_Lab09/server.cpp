#include "server.h"
#include <QDebug>

Server::Server(const QString& path) : server_socket(-1), socket_path(path) {}

Server::~Server() {
    close_server();
}

void Server::create_socket() {
    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_socket == -1) {
        throw(Exception("Failed to create socket"));
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sun_family = AF_UNIX;
    strncpy(server_address.sun_path, socket_path.toUtf8().data(), sizeof(server_address.sun_path) - 1);

    qDebug() << "Server creates socket";
}

void Server::bind_socket(){
    unlink(socket_path.toUtf8().data());

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        throw(Exception("Failed to bind socket"));
    }

    qDebug() << "Server binds socket";
}

void Server::listen_for_clients(int backlog) {
    if (listen(server_socket, backlog) == -1) {
        throw(Exception("Failed to listen on socket"));
    }

    qDebug() << "Server is listening for clients";
}

int Server::accept_client() {
    int client_socket = accept(server_socket, nullptr, nullptr);
    if (client_socket == -1) {
        throw(Exception("Failed to accept client connection"));
    }

    qDebug() << "Server accepts connection with client";

    return client_socket;
}

void Server::send_data(int client_socket, const QString& data) {
    if (send(client_socket, data.toUtf8().data(), data.length(), 0) == -1) {
        throw(Exception("Failed to send data"));
    }
}

QString Server::receive_data(int client_socket) {
    char buffer[1024] = {0};
    int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received == -1) {
        return "";
    }
    return QString(buffer);
}

void Server::close_server() {
    if (server_socket != -1) {
        close(server_socket);
        unlink(socket_path.toUtf8().data());
        server_socket = -1;
    }
}
