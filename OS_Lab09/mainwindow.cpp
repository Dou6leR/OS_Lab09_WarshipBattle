#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    // // func_connections
    // connect(this->ui->player_but, &QPushButton::clicked, this, &MainWindow::test_func);

    //One time init
    CCell::initPixmap();

    scene = new QGraphicsScene;
    //Add first shipField
    shipField1.resize(100);
    for(int x = 0; x < 10; x++)
    {
        for(int y = 0; y < 10; y++)
        {
            shipField1[x + y * 10] = new CCell(_cell);
            shipField1[x + y * 10]->setPos(2 * CCell::SIZE + x * CCell::SIZE, 5 * CCell::SIZE + y * CCell::SIZE);
            scene->addItem(shipField1[x + y * 10]);
        }
    }
    //Add second shipField
    shipField2.resize(100);
    for(int x = 0; x < 10; x++)
    {
        for(int y = 0; y < 10; y++)
        {
            shipField2[x + y * 10] = new CCell(_cell);
            shipField2[x + y * 10]->setPos(800 - 12 * CCell::SIZE + x * CCell::SIZE, 5 * CCell::SIZE + y * CCell::SIZE);
            scene->addItem(shipField2[x + y * 10]);
        }
    }

    //Add all ships to scene
    ships.resize(10);
    int i = 0, j = 1;

    for(int k = 1; k <= j; k++, i++)
    {
        ships[i] = new CCell(_sh_4);
        ships[i]->setPos(CCell::SIZE * (_sh_4 * (k - 1) + k), CCell::SIZE * (10 - j * 2));
        scene->addItem(ships[i]);
    }
    j++;
    for(int k = 1; k <= j; k++, i++)
    {
        ships[i] = new CCell(_sh_3);
        ships[i]->setPos(CCell::SIZE * (_sh_3 * (k - 1) + k), CCell::SIZE * (10 - j * 2));
        scene->addItem(ships[i]);
    }
    j++;
    for(int k = 1; k <= j; k++, i++)
    {
        ships[i] = new CCell(_sh_2);
        ships[i]->setPos(CCell::SIZE * (_sh_2 * (k - 1) + k), CCell::SIZE * (10 - j * 2));
        scene->addItem(ships[i]);
    }
    j++;
    for(int k = 1; k <= j; k++, i++)
    {
        ships[i] = new CCell(_sh_1);
        ships[i]->setPos(CCell::SIZE * (_sh_1 * (k - 1) + k), CCell::SIZE * (10 - j * 2));
        scene->addItem(ships[i]);
    }
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

void MainWindow::on_server_but_clicked()
{
    switch_page(SERVER_PAGE);

    new Server("/tmp/socket", ui->server_log);
}

void MainWindow::on_player_but_clicked()
{
    switch_page(SHIP_PLACE_PAGE);

    new Client("/tmp/socket");
}

void MainWindow::on_ready_but_clicked()
{
    qDebug() << "On Ready Button clicked";
}
