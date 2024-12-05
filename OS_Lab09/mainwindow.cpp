#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*
    try{
        server->create_socket();
        qDebug() << "Server creates socket";

        server->bind_socket();
        qDebug() << "Server binds socket";

        server->listen_for_clients();
        qDebug() << "Server is listening for clients";


        client->create_socket();
        qDebug() << "Client creates socket. Socket: " << client->get_socket();

        client->connect_to_server();
        qDebug() << "Client connects to server";

        int client_socket = server->accept_client();
        qDebug() << "Server accepts connection with client";

        client->send_data("Sending some data from client to server");
        qDebug() << "Client sends data to server";

        std::string data = server->receive_data(client_socket);
        qDebug() << "Server receives data from client. Data: " << data;
    }
    catch(Exception exc){
        qDebug() << exc.what();
    }
    catch(...){
        qDebug() << "Unknown exception occured";
    }
    */

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::switch_page(int page)
{
    switch (page)
    {
        case WELCOME_PAGE:
            this->ui->mw_pages->setCurrentWidget(this->ui->welcome);
            break;
        case SHIP_PLACE_PAGE:
            this->ui->mw_pages->setCurrentWidget(this->ui->ship_place);
            break;
        case SERVER_PAGE:
            this->ui->mw_pages->setCurrentWidget(this->ui->server_page);
            break;
        case GAME_PAGE:
            this->ui->mw_pages->setCurrentWidget(this->ui->game_page);
            break;
        default:
            qDebug() << "Page switch error index";
            //TODO: error or exception
            break;
    }
}
void MainWindow::connect_client(Client* client){
    client->connect_to_server();
}

void MainWindow::connect_server(){
    server->create_socket();
    server->bind_socket();
    server->listen_for_clients();
}

void MainWindow::accept_client(){
    client_sockets.append(server->accept_client());
    qDebug() << "Client accepted" << "\t size = " << client_sockets.size();
}

void MainWindow::send_to_server(Client* client){
    client->send_data("Some text to server");
}

void MainWindow::receive_data(int id){
    QString data;
    data = server->receive_data(client_sockets[id]);

    qDebug() << "Server receives data from client "<< id <<". Data: " << data;
}

void MainWindow::send_client1(){
    server->send_data(client_sockets[0], "Message to client 1");
}

void MainWindow::send_client2(){
    server->send_data(client_sockets[1], "Message to client 2");
}

void MainWindow::receive_from_server(Client* new_client){
    QString data = new_client->receive_data();
    qDebug() << "Message from server: " << data;
}

void MainWindow::on_player_btn_clicked(){

    Client* new_client = new Client("/tmp/socket");
    new_client->create_socket();

    QWidget *new_window = new QWidget();
    new_window->setWindowTitle("Player");
    new_window->resize(800, 600);

    QVBoxLayout *layout = new QVBoxLayout(new_window);
    QPushButton *connect_btn = new QPushButton("Connect", new_window);
    QPushButton *send_btn = new QPushButton("Send", new_window);
    QPushButton *receive_btn = new QPushButton("Receive", new_window);

    layout->addWidget(connect_btn);
    layout->addWidget(send_btn);
    layout->addWidget(receive_btn);


    //connect(connect_btn, &QPushButton::clicked, this, &MainWindow::connect_client);
    connect(connect_btn, &QPushButton::clicked, this, [=]() {
        MainWindow::connect_client(new_client);
    });
    //connect(send_btn, &QPushButton::clicked, this, &MainWindow::send_to_server);
    connect(send_btn, &QPushButton::clicked, this, [=]() {
        MainWindow::send_to_server(new_client);
    });
    connect(receive_btn, &QPushButton::clicked, this, [=]() {
        MainWindow::receive_from_server(new_client);
    });

    this->hide();
    new_window->show();
}

void MainWindow::on_server_btn_clicked()
{
    server = new Server("/tmp/socket");

    QWidget *new_window = new QWidget();
    new_window->setWindowTitle("Server");
    new_window->resize(800, 600);

    QVBoxLayout *layout = new QVBoxLayout(new_window);
    QPushButton *connect_btn = new QPushButton("Connect", new_window);
    QPushButton *accept_btn = new QPushButton("Accept client", new_window);
    QPushButton *receive_btn = new QPushButton("Receive from client", new_window);
    QPushButton *send1_btn = new QPushButton("Send client 1", new_window);
    QPushButton *send2_btn = new QPushButton("Send client 2", new_window);

    layout->addWidget(connect_btn);
    layout->addWidget(accept_btn);
    layout->addWidget(receive_btn);
    layout->addWidget(send1_btn);
    layout->addWidget(send2_btn);

    connect(connect_btn, &QPushButton::clicked, this, &MainWindow::connect_server);
    connect(accept_btn, &QPushButton::clicked, this, &MainWindow::accept_client);
    connect(receive_btn, &QPushButton::clicked, this, &MainWindow::receive_data);
    connect(send1_btn, &QPushButton::clicked, this, &MainWindow::send_client1);
    connect(send2_btn, &QPushButton::clicked, this, &MainWindow::send_client2);

    this->hide();
    new_window->show();
}