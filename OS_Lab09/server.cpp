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
                QMap<int, bool> tmp;
                for(auto c : data_list[i].split(","))
                {
                    qDebug() << c;
                    tmp[c.toInt()] = false;
                }
                client1_ships.append(tmp);
            }
            m_server->put_in_log("Player 1 sent right message. Ship placement: \n" + data);
        }
        else{
            m_server->put_in_log("Player 1 sent wrong type of message");
            throw Exception("Wrong type of message");
        }

        data = m_server->receive_data(client_sockets[1]);
        data_list = data.split(" ");
        if (data_list[0].toInt() == SHIP_PLACEMENT_MSG){
            for(int i = 1; i < data_list.size(); i++){
                //client2_ships.append(QMap<int, bool>());
                //client2_ships[i-1].clear();
                QMap<int, bool> tmp;
                for(auto c : data_list[i].split(","))
                {
                    qDebug() << c;
                    tmp[c.toInt()] = false;
                }
                client2_ships.append(tmp);

            }
            m_server->put_in_log("Player 2 sent right message. Ship placement: \n" + data);
            data = "Vector<Qmap<coord, isHit>>";
            for(auto ship: client2_ships)
            {
                data += "(";
                for(auto it = ship.begin(); it != ship.end(); it++)
                    data += "key = " + QString::number(it.key()) + "\tvalue = " + QString::number(it.value());
                data += ")";
            }
            m_server->put_in_log(data);

        }
        else{
            m_server->put_in_log("Player 2 sent wrong type of message");
            throw Exception("Wrong type of message");
        }

        bool turn = false ;//QRandomGenerator::global()->bounded(2);

        data = QString::number(READY_MSG) + " 1";
        m_server->send_data(client_sockets[turn], data);
        m_server->put_in_log("Sent ready message: " + data);
        data = QString::number(READY_MSG) + " 0";
        m_server->send_data(client_sockets[!turn], data);
        m_server->put_in_log("Sent ready message: " + data);

        while(true){
            data = m_server->receive_data(client_sockets[turn]);
            m_server->put_in_log("shot cell: " + data + "\nSocket: " + QString::number(client_sockets[turn]));
            data_list = data.split(" ");
            if (data_list[0].toInt() != SHOOT_MSG){
                m_server->put_in_log("Wrong type of message " + data + "\nSocket: " + QString::number(client_sockets[turn]));
                throw Exception("Wrong type of message");
            }
            bool is_killed;
            bool is_missed = true;
            for(auto ship : (turn ? client1_ships : client2_ships )){
                if(ship.contains(data_list[1].toInt())){

                    is_missed = false;
                    if(ship[(data_list[1].toInt())] == false)
                        ship[(data_list[1].toInt())] = true;

                    for(auto ship1: (turn ? client1_ships : client2_ships))
                    {
                        data += "(";
                        for(auto it = ship1.begin(); it != ship1.end(); it++)
                            data += "key = " + QString::number(it.key()) + "\tvalue = " + QString::number(it.value()) + "\n";
                        data += ")";
                    }
                    m_server->put_in_log(data);
                    is_killed = check_killed(ship);
                    if (is_killed){
                        data = QString::number(SHOOTER_KILL_MSG);
                        m_server->put_in_log("kill message shooter: " + data + "\nSocket: " + QString::number(client_sockets[turn]));
                        for(auto cord : ship.keys()){
                            data += " " + QString::number(cord);
                        }
                        m_server->send_data(client_sockets[turn], data);

                        data = QString::number(RECEIVER_KILL_MSG);
                        m_server->put_in_log("kill message receiver: " + data + "\nSocket: " + QString::number(client_sockets[!turn]));
                        for(auto cord : ship.keys()){
                            data += " " + QString::number(cord);
                        }
                        m_server->send_data(client_sockets[!turn], data);
                        break;

                    }
                    else{
                        data = QString::number(TO_SHOOTER_HIT_MSG) + " " + data_list[1];
                        m_server->put_in_log("hit message shooter: " + data + "\nSocket: " + QString::number(client_sockets[turn]));
                        m_server->send_data(client_sockets[turn], data);

                        data = QString::number(TO_RECEIVER_HIT_MSG) +  " " + data_list[1];
                        m_server->put_in_log("hit message receiver: " + data + "\nSocket: " + QString::number(client_sockets[!turn]));
                        m_server->send_data(client_sockets[!turn], data);
                        break;
                    }
                    break;
                }
            }
            if (is_missed){
                data = QString::number(TO_SHOOTER_MISS_MSG) +  " " + data_list[1];
                m_server->put_in_log("miss message shooter: " + data + "\nSocket: " + QString::number(client_sockets[turn]));
                m_server->send_data(client_sockets[turn], data);

                data = QString::number(TO_RECEIVER_MISS_MSG) +  " " + data_list[1];
                m_server->put_in_log("miss message receiver: " + data + "\nSocket: " + QString::number(client_sockets[!turn]));
                m_server->send_data(client_sockets[!turn], data);
            }
        }
    }
    catch(std::exception exc)
    {
        qDebug() << exc.what();
    }
}

bool ServerController::check_killed(QMap<int,bool> ship){
    for(bool cord : ship.values()){
        if(cord == false){
            return false;
        }
    }
    return true;
}
