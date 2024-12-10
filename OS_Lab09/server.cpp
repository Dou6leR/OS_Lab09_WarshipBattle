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

Fleet::Fleet() {}
Fleet::~Fleet() {}

void Fleet::set_fleet(QString source)
{
    QList<QString> data_list = source.split(" ");
    ships.resize(data_list.count()-1);
    for(int i = 1; i < data_list.size(); i++){
        QList<QString> ship_list = data_list[i].split(",");
        for (int coord_index = 0; coord_index < ship_list.size(); coord_index++)
        {
            ships[i-1].append(QPair<int, bool>(ship_list[coord_index].toInt(), false));
        }
    }
}

QString Fleet::output_fleet() const
{
    QString result = "";
    for (int i = 0; i < ships.count(); i++)
    {
        for (int j = 0; j < ships[i].count(); j++)
        {
            result += "( " + QString::number(ships[i][j].first)
                      + " , " + QString::number(ships[i][j].second) + " )";
        }
        result += "\n";
    }
    return result;
}

int Fleet::hit_if_exist(int coord)
{
    for (int i = 0; i < ships.count(); i++)
    {
        for (int j = 0; j < ships[i].count(); j++)
        {
            if (coord == ships[i][j].first)
            {
                ships[i][j].second = true;
                if (is_killed(ships[i]))
                {
                    num_of_killed++;
                    if(is_win())
                        return 3;
                    return 2;
                }
                else
                    return 1;
            }
        }
    }
    return 0;
}

bool Fleet::is_killed(QVector<QPair<int, bool>> ship)
{
    for(int j = 0; j < ship.count(); j++){
        if(ship[j].second == false){
            return false;
        }
    }
    return true;
}

bool Fleet::is_win()
{
    if(num_of_killed == 10)
        return true;
    return false;
}

QString Fleet::get_ship(int coord) const
{
    for (int i = 0; i < ships.count(); i++)
    {
        for (int j = 0; j < ships[i].count(); j++)
        {
            if (coord == ships[i][j].first)
            {
                QString result = "";
                for (int k = 0; k < ships[i].count(); k++)
                {
                    result += QString::number(ships[i][k].first) + " ";
                }
                result.removeLast();
                return result;
            }
        }
    }
    return "";
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
        QString client1_ship_placement = m_server->receive_data(client_sockets[0]);
        QList<QString> data_list = client1_ship_placement.split(" ");
        if (data_list[0].toInt() == SHIP_PLACEMENT_MSG){
            client1_ships.set_fleet(client1_ship_placement);
            m_server->put_in_log("Player 1 sent right message. Ship placement: \n" + client1_ship_placement);
        }
        else{
            m_server->put_in_log("Player 1 sent wrong type of message");
            throw Exception("Wrong type of message");
        }

        QString client2_ship_placement = m_server->receive_data(client_sockets[1]);
        data_list = client2_ship_placement.split(" ");
        if (data_list[0].toInt() == SHIP_PLACEMENT_MSG){
            client2_ships.set_fleet(client2_ship_placement);
            m_server->put_in_log("Player 2 sent right message. Ship placement: \n" + client2_ship_placement);
        }
        else{
            m_server->put_in_log("Player 2 sent wrong type of message");
            throw Exception("Wrong type of message");
        }

        // output to log fleet classes
        m_server->put_in_log("Fleet 1 is:\n" + client1_ships.output_fleet());
        m_server->put_in_log("Fleet 2 is:\n" + client2_ships.output_fleet());


        // TO CHANGE
        bool turn = false ;//QRandomGenerator::global()->bounded(2);

        QString data = QString::number(READY_MSG) + " 1";
        m_server->send_data(client_sockets[turn], data);
        m_server->put_in_log("Sent ready message: " + data);
        data = QString::number(READY_MSG) + " 0";
        m_server->send_data(client_sockets[!turn], data);
        m_server->put_in_log("Sent ready message: " + data);



        // TO CHANGE END

        while(true){
            data = m_server->receive_data(client_sockets[turn]);
            m_server->put_in_log("shot cell: " + data + "\nSocket: " + QString::number(client_sockets[turn]));
            data_list = data.split(" ");
            if (data_list[0].toInt() != SHOOT_MSG){
                m_server->put_in_log("Wrong type of message " + data + "\nSocket: " + QString::number(client_sockets[turn]));
                throw Exception("Wrong type of message");
            }
            Fleet* current_fleet = turn ? &client1_ships : &client2_ships;
            Fleet* enemy_fleet = !turn ? &client1_ships : &client2_ships;
            int coord = data_list[1].toInt();
            int shoot_result = current_fleet->hit_if_exist(coord);
            QString str_ship;
            //0 - miss 1 - hit 2 - kill 3 - win
            switch (shoot_result)
            {
            case 0:
                send_message(TO_SHOOTER_MISS_MSG, data_list[1], turn, "to shooter miss: ");
                send_message(TO_RECEIVER_MISS_MSG, data_list[1], !turn, "to receiver miss: ");
                turn = !turn;
                break;

            case 1:
                send_message(TO_SHOOTER_HIT_MSG, data_list[1], turn, "to shooter hit: ");
                send_message(TO_RECEIVER_HIT_MSG, data_list[1], !turn, "to receiver hit: ");
                m_server->put_in_log("Hit:\n" + current_fleet->output_fleet());
                break;

            case 2:
                str_ship = (current_fleet->get_ship(coord));
                send_message(SHOOTER_KILL_MSG, str_ship, turn, "to shooter kill: ");
                send_message(RECEIVER_KILL_MSG, str_ship, !turn, "to receiver kill: ");
                m_server->put_in_log("Kill:\n" + current_fleet->output_fleet());
                break;

            case 3:
                str_ship = (current_fleet->get_ship(coord));
                send_message(WIN_MSG, str_ship, turn, "to shooter win: ");

                send_message(LOSE_MSG, enemy_fleet->to_loser_msg(str_ship), !turn, "to receiver lose: ");

                m_server->put_in_log("End of the game:\n" + current_fleet->output_fleet());
                break;

            default:
                break;
            }
        }
    }
    catch(std::exception exc)
    {
        qDebug() << exc.what();
    }
}

void ServerController::send_message(int type, QString message, bool turn, QString message_prefix)
{
    QString data = QString::number(type) +  " " + message;
    m_server->put_in_log(message_prefix + data + "\nSocket: " + QString::number(client_sockets[turn]));
    m_server->send_data(client_sockets[turn], data);
}

QString Fleet::to_loser_msg(QString str_ship){
    QString data = str_ship + " ";
    for (auto ship : ships){
        if (!is_killed(ship)){
            for(auto cell : ship){
                data += QString::number(cell.first) + ":" + QString::number(cell.second) + ",";
            }
            data.removeLast();
            data += "_";
        }
    }
    return data.removeLast();
}






























