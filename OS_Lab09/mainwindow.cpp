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

    connect(server, &Server::log_signal, this, &MainWindow::put_in_log);
}

void MainWindow::on_player_but_clicked()
{
    initShipsAndGrids();

    switch_page(SHIP_PLACE_PAGE);

    client = new Client("/tmp/socket");

    connections_init();
}

void MainWindow::on_ready_but_clicked()
{
    if(ships->checkAllConection())
        return;

    ships->getAllShipPositions();

    switch_page(GAME_PAGE);
}

void MainWindow::put_in_log(QString log){
    ui->server_log->moveCursor(QTextCursor::End);
    ui->server_log->textCursor().insertText("\n");
    ui->server_log->textCursor().insertText(get_log_date_time());
    ui->server_log->textCursor().insertText(log);
    ui->server_log->moveCursor(QTextCursor::End);
}

QString MainWindow::get_log_date_time()
{
    QString result = "[ ";
    QDate date = QDate::currentDate();
    QString s_date = QString("%1/%2/%3")
                .arg( date.day(), 2, 10, QChar('0'))
                .arg( date.month(), 2, 10, QChar('0'))
                .arg( date.year(), 4, 10, QChar('0'));

    result += s_date + " ";

    QTime time = QTime::currentTime();
    QString s_time = QString("%1:%2:%3:%4")
                         .arg( time.hour(), 2, 10, QChar('0'))
                         .arg( time.minute(), 2, 10, QChar('0'))
                         .arg( time.second(), 2, 10, QChar('0'))
                         .arg( time.msec(), 3, 10, QChar('0'));

    result += s_time + " ] ";
    return result;
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

void MainWindow::connections_init(){
    connect(ui->random_but, &QPushButton::clicked, ships, &CShip::randomShipsPositions);
    connect(ships, &CShip::ShipPositions, client, &Client::send_ship_positions);
    connect(grid2, &CGrid::sendCellToServer, client, &Client::send_shoot);
    connect(client, &Client::to_shooter_hit_msg, grid2, &CGrid::recieveHitAttacker, Qt::QueuedConnection);
    connect(client, &Client::to_shooter_miss_msg, grid2, &CGrid::recieveMissAttacker, Qt::QueuedConnection);
    connect(client, &Client::shooter_kill_msg, grid2, &CGrid::recieveKillAttacker, Qt::QueuedConnection);
    connect(client, &Client::to_receiver_hit_msg, grid1, &CGrid::recieveHitDefender, Qt::QueuedConnection);
    //connect(client, &Client::to_receiver_miss_msg, grid2, &CGrid::startRecievingShoots, Qt::QueuedConnection);
    connect(client, &Client::to_receiver_miss_msg, grid1, &CGrid::recieveMissDefender, Qt::QueuedConnection);
    connect(client, &Client::receiver_kill_msg, grid1, &CGrid::recieveKillDefender, Qt::QueuedConnection);
    connect(client, &Client::ready_msg, grid2, &CGrid::startRecievingShoots, Qt::QueuedConnection);
}
