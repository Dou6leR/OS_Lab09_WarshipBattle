#include "client.h"
#include <QDebug>

ClientController::ClientController(Client *client): m_client(client){}

Client::Client(const QString& path) : socket_path(path), client_socket(-1) {
    client_init();
}

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
    if (::connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
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

void Client::client_init(){
    QThread* thread = new QThread();
    ClientController* client_ctrl = new ClientController(this);
    client_ctrl->moveToThread(thread);
    connect( thread, &QThread::started, client_ctrl, &ClientController::process_client);
    connect( client_ctrl, &ClientController::finished, thread, &QThread::quit);
    connect( client_ctrl, &ClientController::finished, client_ctrl, &ClientController::deleteLater);
    connect( thread, &QThread::finished, thread, &QThread::deleteLater);
    thread->start();
}

void ClientController::process_client(){
    try
    {
        m_client->create_socket();

        m_client->connect_to_server();

        while (true)
        {
            QString data = m_client->receive_data();
            qDebug() << "Message from server: " << data;
            sleep(5);
            m_client->send_data("Message from client");
        }
    }
    catch(std::exception exc)
    {
        qDebug() << exc.what();
    }
}
