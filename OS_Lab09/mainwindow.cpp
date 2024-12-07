#include "mainwindow.h"
#include "ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    ui->placing_View->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui->placing_View->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui->game_View->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui->game_View->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    // ui-game_View->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    // ui->game_View->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    //One time init
    CCell::initPixmap();

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

void MainWindow::on_server_but_clicked()
{
    switch_page(SERVER_PAGE);

    server = new Server("/tmp/socket");

    server->create_socket();
    server->bind_socket();
    server->listen_for_clients();

    client_sockets.append(server->accept_client());

    client_sockets.append(server->accept_client());

    // QTimer* timer = new QTimer(this);
    // connect(timer, &QTimer::timeout, this, [this]() {
    //     QString data = "Your turn";

    //     server->send_data(client_sockets[0], data);
    //     data = server->receive_data(client_sockets[0]);
    //     qDebug() << "Message from client 1: " << data;

    //     server->send_data(client_sockets[1], data);
    //     data = server->receive_data(client_sockets[1]);
    //     qDebug() << "Message from client 2: " << data;
    // });

    // timer->start(5000);
}

void MainWindow::on_player_but_clicked()
{
    initShipsAndGrids();
    ui->placing_View->setScene(scene);
    switch_page(SHIP_PLACE_PAGE);

    Client* new_client = new Client("/tmp/socket");

    new_client->create_socket();

    new_client->connect_to_server();

    // QTimer* timer = new QTimer(this);
    // connect(timer, &QTimer::timeout, this, [new_client]() {
    //     QString data = new_client->receive_data();
    //     qDebug() << "Message from server: " << data;
    //     sleep(5);
    //     new_client->send_data("Message from client");
    // });

    // timer->start(5000);
}


void MainWindow::on_ready_but_clicked()
{
    for(int i = 0; i < SHIPSNUM;  i++)
    {
        if(!(ships[i]->isConToTable))
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Attention!");
            msgBox.setText("Place all ships first and check if all ships in connected to the grid");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Information);
            msgBox.exec();
            return;
        }
    }
    //To Do:
    //Send all ships to the server
    //PositionOfShip array of all indexes ships[i]->PositionOfShip




    switch_page(GAME_PAGE);
    for(int i = 0; i < 100; i++)
    {
        shipField1[i]->show();
    }
    for(int i = 0; i < SHIPSNUM; i++)
    {
        ships[i]->deleteMovableOption();
        ships[i]->moveBy(-13 * CCell::SIZE, 0);
    }
    ui->game_View->setScene(scene);
}

void MainWindow::initShipsAndGrids()
{
    scene = new QGraphicsScene;
    //Add first shipField
    shipField1.resize(100);
    for(int x = 0; x < 10; x++)
    {
        for(int y = 0; y < 10; y++)
        {
            shipField1[x + y * 10] = new CCell(_cell, 0);
            shipField1[x + y * 10]->setPos(1 * CCell::SIZE + x * CCell::SIZE, 3 * CCell::SIZE + y * CCell::SIZE);
            scene->addItem(shipField1[x + y * 10]);
            shipField1[x + y * 10]->hide();
        }
    }
    //Add second shipField
    shipField2.resize(100);
    for(int x = 0; x < 10; x++)
    {
        for(int y = 0; y < 10; y++)
        {
            shipField2[x + y * 10] = new CCell(_cell, 0);
            shipField2[x + y * 10]->setPos(14 * CCell::SIZE + x * CCell::SIZE, 3 * CCell::SIZE + y * CCell::SIZE);
            scene->addItem(shipField2[x + y * 10]);
        }
    }

    //Add all ships to scene
    ships.resize(10);
    int i = 0, j = 1;

    for(int k = 1; k <= j; k++, i++)
    {
        ships[i] = new CCell(_sh_4, 1);
        ships[i]->setPos(CCell::SIZE * (_sh_4 * (k - 1) + k), CCell::SIZE * (10 - j * 2));
        scene->addItem(ships[i]);
    }
    j++;
    for(int k = 1; k <= j; k++, i++)
    {
        ships[i] = new CCell(_sh_3, 1);
        ships[i]->setPos(CCell::SIZE * (_sh_3 * (k - 1) + k), CCell::SIZE * (10 - j * 2));
        scene->addItem(ships[i]);
    }
    j++;
    for(int k = 1; k <= j; k++, i++)
    {
        ships[i] = new CCell(_sh_2, 1);
        ships[i]->setPos(CCell::SIZE * (_sh_2 * (k - 1) + k), CCell::SIZE * (10 - j * 2));
        scene->addItem(ships[i]);
    }
    j++;
    for(int k = 1; k <= j; k++, i++)
    {
        ships[i] = new CCell(_sh_1, 1);
        ships[i]->setPos(CCell::SIZE * (_sh_1 * (k - 1) + k), CCell::SIZE * (10 - j * 2));
        scene->addItem(ships[i]);
    }

    // fix for QGraphicScene
    QRectF view_rect = scene->itemsBoundingRect();
    view_rect.adjust(0, 0, 0, 0);
    scene->setSceneRect(view_rect);
}
