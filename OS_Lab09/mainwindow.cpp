#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    server = new Server("/tmp/socket");
    client = new Client("/tmp/socket");


    //TODO: replace these if/else with exceptions
    if(!server->create_socket()){
        qDebug() << "Error creating server socket";
    }
    else {
        qDebug() << "Server creates socket";
    }
    if(!server->bind_socket()){
        qDebug() << "Error binding socket";
    }
    else {
        qDebug() << "Server binds socket";
    }
    if(!server->listen_for_clients()){
        qDebug() << "Error listening for clients";
    }
    else {
        qDebug() << "Server is listening for clients";
    }




    if(!client->create_socket()){
        qDebug() << "Error creating client socket";
    }
    else {
        qDebug() << "Client creates socket. Socket: " << client->get_socket();
    }
    if(!client->connect_to_server()){
        qDebug() << "Error connecting to server";
    }
    else {
        qDebug() << "Client connects to server";
    }
    int client_socket = server->accept_client();
    qDebug() << "Client connects to server";





    if(client->send_data("Sending some data from client to server")){
        qDebug() << "Client sends data to server";
    }
    else {
        qDebug() << "Client fails to send data to server";
    }
    std::string data = server->receive_data(client_socket);
    if (data == ""){
        qDebug() << "Failed to receive data";
    }
    else{
        qDebug() << "Server receives data from client. Data: " << data;
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}
