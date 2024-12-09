#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);

    switch_page(WELCOME_PAGE);

    ui->placing_View->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui->placing_View->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui->game_View->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui->game_View->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
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
        ui->placing_View->setScene(scene);
        this->ui->mw_pages->setCurrentWidget(this->ui->ship_place);
        break;

    case SERVER_PAGE:
        this->ui->mw_pages->setCurrentWidget(this->ui->server_page);
        break;

    case GAME_PAGE:
        grid1->show();
        grid2->SetCoolCursor();
        ships->SwitchGridForShip();
        ui->game_View->setScene(scene);
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
  
    server = new Server("/tmp/socket", ui->server_log);
}

void MainWindow::on_player_but_clicked()
{
    initShipsAndGrids();

    switch_page(SHIP_PLACE_PAGE);

    client = new Client("/tmp/socket");
}

void MainWindow::on_ready_but_clicked()
{
    if(ships->checkAllConection())
        return;

    //connect send_ship_positions slot to ship_positions signal


    switch_page(GAME_PAGE);
    grid2->startRecievingShoots(); // In future move to server signal when all players are ready

}

void MainWindow::initShipsAndGrids()
{
    scene = new QGraphicsScene;

    grid1 = new CGrid(scene, CCell::SIZE, 3 * CCell::SIZE);
    grid1->hide();
    grid2 = new CGrid(scene, 14 * CCell::SIZE, 3 * CCell::SIZE);
    ships = new CShip(scene, CCell::SIZE, CCell::SIZE);

    // fix for QGraphicScene
    QRectF view_rect = scene->itemsBoundingRect();
    view_rect.adjust(0, 0, 0, 0);
    scene->setSceneRect(view_rect);
}
