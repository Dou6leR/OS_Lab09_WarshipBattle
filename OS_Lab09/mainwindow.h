#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

//UI managing
#define W_WIDTH 800
#define W_HIGHT 600
enum MW_PAGES {
    WELCOME_PAGE,
    SHIP_PLACE_PAGE,
    SERVER_PAGE,
    GAME_PAGE,
};


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void switch_page(int page);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
