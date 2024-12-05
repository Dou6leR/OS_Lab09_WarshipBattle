#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
