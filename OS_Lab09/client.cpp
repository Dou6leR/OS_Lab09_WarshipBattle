#include "client.h"
#include <QDebug>

Client::Client(const QString& path) : socket_path(path), client_socket(-1) {}

Client::~Client() {
    close_connection();
}

void Client::create_socket() {
    client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_socket < 0) {
        throw(Exception("Failed to create socket"));
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sun_family = AF_UNIX;
    strncpy(server_address.sun_path, socket_path.toUtf8().data(), sizeof(server_address.sun_path) - 1);
    qDebug() << "Client creates socket. Socket: " << client_socket;
}

void Client::connect_to_server() {
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        throw(Exception("Connection to server failed"));
    }
    qDebug() << "Client connects to server";
}

void Client::send_data(const QString& data) {
    if (send(client_socket, data.toUtf8().data(), data.length(), 0) == -1) {
        throw(Exception("Failed to send data"));
    }
    qDebug() << "Client sends data to server";
}

QString Client::receive_data() {
    char buffer[1024] = {0};
    int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received == -1) {
        throw(Exception("Failed to receive data"));
    }
    return QString(buffer);
}

void Client::close_connection() {
    if (client_socket != -1) {
        close(client_socket);
        client_socket = -1;
    }
}

int Client::get_socket(){
    return client_socket;
}
