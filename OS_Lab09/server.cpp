#include "server.h"
#include <QDebug>

ServerController::ServerController(Server *server): m_server(server){}

Server::Server(const QString& path, QTextEdit* _server_log)
    : server_log(_server_log)
    , server_socket(-1)
    , socket_path(path)
{
    server_init();
}

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

void Server::server_init(){
    QThread* thread = new QThread();
    ServerController* server_ctrl = new ServerController(this);
    server_ctrl->moveToThread(thread);
    connect( thread, &QThread::started, server_ctrl, &ServerController::process_server);
    connect( server_ctrl, &ServerController::finished, thread, &QThread::quit);
    connect( server_ctrl, &ServerController::finished, server_ctrl, &ServerController::deleteLater);
    connect( thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(server_ctrl, &ServerController::log_signal, this, &Server::put_in_log, Qt::QueuedConnection);

    thread->start();
}

void Server::put_in_log(QString log){
    emit log_signal(log);
}

void ServerController::process_server(){
    try{
        m_server->create_socket();
        m_server->put_in_log("Socket is created");
        m_server->bind_socket();
        m_server->put_in_log("Socket is binded");
        m_server->listen_for_clients();
        m_server->put_in_log("Server listens for clients");

        client_sockets.append(m_server->accept_client());
        m_server->put_in_log("First client accepted. Socket: " + QString::number(client_sockets[0]));

        client_sockets.append(m_server->accept_client());
        m_server->put_in_log("Second client accepted. Socket: " + QString::number(client_sockets[1]));

        // Converting char* message to QVector<QMap<int(position of ship), bool(true is hit)>>
        QString data = m_server->receive_data(client_sockets[0]);
        QList<QString> data_list = data.split(" ");
        if (data_list[0].toInt() == SHIP_PLACEMENT_MSG){
            for(int i = 1; i < data_list.size(); i++){
                client1_ships.append(QMap<int,bool>());
                for(auto c : data_list[i].split(",")){
                    client1_ships[i].insert(c.toInt(), false);
                }
            }
        }
        else{
            throw Exception("Wrong type of message");
        }

        data = m_server->receive_data(client_sockets[1]);
        data_list = data.split(" ");
        if (data_list[0].toInt() == SHIP_PLACEMENT_MSG){
            for(int i = 1; i < data_list.size(); i++){
                client2_ships.append(QMap<int, bool>());
                for(auto c : data_list[i].split(",")){
                    client2_ships[i].insert(c.toInt(), false);
                }
            }
        }
        else{
            throw Exception("Wrong type of message");
        }

        while(true){
            data = "Your turn";

            m_server->send_data(client_sockets[0], data);
            data = m_server->receive_data(client_sockets[0]);
            qDebug() << "Message from client 1: " << data;

            m_server->send_data(client_sockets[1], data);
            data = m_server->receive_data(client_sockets[1]);
            qDebug() << "Message from client 2: " << data;
        }
    }
    catch(std::exception exc)
    {
        qDebug() << exc.what();
    }
}
