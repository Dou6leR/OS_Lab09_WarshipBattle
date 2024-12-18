#include "client.h"
#include <QDebug>

QMutex mutex;
QWaitCondition condition;
bool wait_for_msg;
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
    connect(this, &Client::wait_unlock, client_ctrl, &ClientController::wait_unlock);

    connect(client_ctrl, &ClientController::to_receiver_hit_msg, this, &Client::s_to_receiver_hit_msg, Qt::QueuedConnection);
    connect(client_ctrl, &ClientController::to_receiver_miss_msg, this, &Client::s_to_receiver_miss_msg, Qt::QueuedConnection);
    connect(client_ctrl, &ClientController::receiver_kill_msg, this, &Client::receiver_kill_msg, Qt::QueuedConnection);
    connect(client_ctrl, &ClientController::to_shooter_hit_msg, this, &Client::s_to_shooter_hit_msg, Qt::QueuedConnection);
    connect(client_ctrl, &ClientController::to_shooter_miss_msg, this, &Client::s_to_shooter_miss_msg, Qt::QueuedConnection);
    connect(client_ctrl, &ClientController::shooter_kill_msg, this, &Client::s_shooter_kill_msg, Qt::QueuedConnection);
    connect(client_ctrl, &ClientController::ready_msg, this, &Client::s_ready_msg, Qt::QueuedConnection);
    connect(client_ctrl, &ClientController::win_msg, this, &Client::s_win_msg, Qt::QueuedConnection);
    connect(client_ctrl, &ClientController::lose_msg, this, &Client::s_lose_msg, Qt::QueuedConnection);

    thread->start();
}

void ClientController::wait_unlock(){
    qDebug() << "wait_for_msg = true";
    mutex.lock();
    wait_for_msg = true;
    condition.wakeOne();
    mutex.unlock();
}

void ClientController::process_client(){
    try
    {
        m_client->create_socket();

        m_client->connect_to_server();

        wait_for_msg = false;
        int message_type = -1;
        QVector<int> ship_cord;
        QString data;
        QList<QString> data_list;
        do{
            qDebug() << wait_for_msg;
            mutex.lock();
            while(!wait_for_msg)
            {
                condition.wait(&mutex);
            }
            mutex.unlock();
            if(wait_for_msg){
                qDebug() << "AAAAAAAAAA Entered waiting for message";
                data = m_client->receive_data();
                qDebug() << "Receive response from server about shoot: "<< data;
                data_list = data.split(" ");
                message_type = data_list[0].toInt();
                for(int i = 0; i < data_list.size(); i++)
                    qDebug() << i << ": " << data_list[i];
                switch(message_type){
                case TO_SHOOTER_HIT_MSG:      //I am a shooter
                    emit to_shooter_hit_msg(data_list[1].toInt()); //lock
                    mutex.lock();
                    wait_for_msg = false;
                    mutex.unlock();
                    break;
                case TO_SHOOTER_MISS_MSG:
                    emit to_shooter_miss_msg(data_list[1].toInt()); //ne lock
                    mutex.lock();
                    wait_for_msg = true;
                    condition.wakeOne();
                    mutex.unlock();
                    break;
                case TO_RECEIVER_HIT_MSG:
                    emit to_receiver_hit_msg(data_list[1].toInt()); //ne lock
                    mutex.lock();
                    wait_for_msg = true;
                    condition.wakeOne();
                    mutex.unlock();
                    break;
                case TO_RECEIVER_MISS_MSG:
                    emit to_receiver_miss_msg(data_list[1].toInt()); //lock
                    mutex.lock();
                    wait_for_msg = false;
                    mutex.unlock();
                    break;
                case SHOOTER_KILL_MSG:
                    for(int i = 1; i< data_list.size(); i++){       //lock
                        ship_cord.append(data_list[i].toInt());
                    }
                    emit shooter_kill_msg(ship_cord);
                    ship_cord.clear();
                    mutex.lock();
                    wait_for_msg = false;
                    mutex.unlock();
                    break;
                case RECEIVER_KILL_MSG:
                    for(int i = 1; i< data_list.size(); i++){       //ne lock
                        ship_cord.append(data_list[i].toInt());
                    }
                    emit receiver_kill_msg(ship_cord);
                    ship_cord.clear();
                    mutex.lock();
                    wait_for_msg = true;
                    condition.wakeOne();
                    mutex.unlock();
                    break;
                case READY_MSG:
                    qDebug() << data;
                    emit ready_msg(data_list[1].toInt());
                    if(data_list[1].toInt()){

                        mutex.lock();
                        wait_for_msg = false;
                        mutex.unlock();
                    }
                    else{
                        mutex.lock();
                        wait_for_msg = true;
                        condition.wakeOne();
                        mutex.unlock();
                    }
                    break;
                default:
                    wait_for_msg = false;
                    break;
                }
            }
        }
        while((message_type != WIN_MSG) && (message_type != LOSE_MSG));

        if (message_type == WIN_MSG){
            for(int i = 1; i< data_list.size(); i++){       //lock
                ship_cord.append(data_list[i].toInt());
            }
            emit shooter_kill_msg(ship_cord);
            ship_cord.clear();
            emit win_msg();
        }
        else{
            for(int i = 1; i < data_list.size()-1; i++){       //ne lock
                ship_cord.append(data_list[i].toInt());
            }
            emit receiver_kill_msg(ship_cord);
            ship_cord.clear();

            emit lose_msg(lose_message(data_list.last()));
        }
    }
    catch(std::exception exc)
    {
        qDebug() << exc.what();
    }
}

QVector<QVector<QPair<int,bool>>> ClientController::lose_message(QString data){
    QList<QString> remaining_ships_list = data.split("_");
    QVector<QVector<QPair<int,bool>>> remaining_ships;
    remaining_ships.resize(remaining_ships_list.size());
    for(int i = 0; i < remaining_ships_list.size(); i++){
        QList<QString> ship_list = remaining_ships_list[i].split(",");
        for (int j = 0; j < ship_list.size(); j++)
        {
            QList<QString> cell_data = ship_list[j].split(":");
            remaining_ships[i].append(QPair<int, bool>(cell_data[0].toInt(), cell_data[1].toInt()));
        }
    }
    return remaining_ships;
}

void Client::send_shoot(int n){
    QString data = QString::number(SHOOT_MSG);
    data += " " + QString::number(n);
    qDebug() << "ShootDAta: " << data;
    mutex.lock();
    wait_for_msg = true;
    condition.wakeOne();
    mutex.unlock();
    send_data(data);
    //emit wait_unlock();

}

void Client::send_ship_positions(QString positions){
    QString data = QString::number(SHIP_PLACEMENT_MSG);
    data += " " + positions;
    qDebug() << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << data;
    //emit wait_unlock();

    mutex.lock();
    wait_for_msg = true;
    condition.wakeOne();
    mutex.unlock();
    send_data(data);
}


void Client::s_to_shooter_hit_msg(int position)
{
    emit to_shooter_hit_msg(position);
}

void Client::s_to_shooter_miss_msg(int position)
{
    emit to_shooter_miss_msg(position);
}

void Client::s_to_receiver_hit_msg(int position)
{
    qDebug() << position;
    emit to_receiver_hit_msg(position);
}

void Client::s_to_receiver_miss_msg(int position)
{
    emit to_receiver_miss_msg(position);
}

void Client::s_shooter_kill_msg(QVector<int> ship)
{
    emit shooter_kill_msg(ship);
}

void Client::s_receiver_kill_msg(QVector<int> ship)
{
    emit receiver_kill_msg(ship);
}

void Client::s_ready_msg(bool turn)
{
    qDebug() << "My turn:";
    qDebug() << turn;
    emit ready_msg(turn);
}

void Client::s_win_msg()
{
    emit win_msg();
}

void Client::s_lose_msg(QVector<QVector<QPair<int,bool>>> remaining_ships)
{

    emit lose_msg(remaining_ships);
}
