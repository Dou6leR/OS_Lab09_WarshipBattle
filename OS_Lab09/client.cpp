#include "client.h"

Client::Client(const std::string& path) : socket_path(path), client_socket(-1) {}

Client::~Client() {
    close_connection();
}

bool Client::create_socket() {
    client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Failed to create socket");
        return false;
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sun_family = AF_UNIX;
    strncpy(server_address.sun_path, socket_path.c_str(), sizeof(server_address.sun_path) - 1);

    return true;
}

bool Client::connect_to_server() {
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Connection to server failed");
        return false;
    }
    return true;
}

bool Client::send_data(const std::string& data) {
    if (send(client_socket, data.c_str(), data.length(), 0) == -1) {
        perror("Failed to send data");
        return false;
    }
    return true;
}

std::string Client::receive_data() {
    char buffer[1024] = {0};
    int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received == -1) {
        perror("Failed to receive data");
        return "";
    }
    return std::string(buffer, bytes_received);
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
