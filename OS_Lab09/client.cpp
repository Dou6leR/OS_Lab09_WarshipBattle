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

        int message_type;
        QVector<int> ship_cord;
        do{
            QString data = m_client->receive_data();
            QList<QString> data_list = data.split(" ");
            message_type = data_list[0].toInt();
            switch(message_type){
            case TO_SHOOTER_HIT_MSG:      //I am a shooter
                emit to_shooter_hit_msg(data_list[1].toInt());
                break;
            case TO_SHOOTER_MISS_MSG:
                emit to_shooter_miss_msg(data_list[1].toInt());
                break;
            case TO_RECEIVER_HIT_MSG:
                emit to_receiver_hit_msg(data_list[1].toInt());
                break;
            case TO_RECEIVER_MISS_MSG:
                emit to_receiver_miss_msg(data_list[1].toInt());
                break;
            case SHOOTER_KILL_MSG:
                for(int i = 1; i< data_list.size(); i++){
                    ship_cord.append(data_list[i].toInt());
                }
                emit shooter_kill_msg(ship_cord);
                ship_cord.clear();
                break;
            case RECEIVER_KILL_MSG:
                for(int i = 1; i< data_list.size(); i++){
                    ship_cord.append(data_list[i].toInt());
                }
                emit receiver_kill_msg(ship_cord);
                ship_cord.clear();
                break;
            case READY_MSG:
                emit ready_msg(data_list[1].toInt());
                break;
            }
        }
        while((message_type != WIN_MSG) && (message_type != LOSE_MSG));
        if (message_type == WIN_MSG){
            emit win_lose_msg(1);
        }
        else{
            emit win_lose_msg(0);
        }
    }
    catch(std::exception exc)
    {
        qDebug() << exc.what();
    }
}

void Client::send_shoot(int n){
    QString data = QString::number(SHOOT_MSG);
    data += " " + QString::number(n);
    send_data(data);
}

void Client::send_ship_positions(QString positions){
    QString data = QString::number(SHIP_PLACEMENT_MSG);
    data += " " + positions;
    send_data(data);
}
