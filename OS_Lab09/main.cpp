#include "mainwindow.h"

#include <QApplication>
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlContext>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // MainWindow w;
    // w.show();
    QQmlEngine engine;
    QQmlContext *objectContext = new QQmlContext(engine.rootContext());

    QQmlComponent component(&engine, QUrl(QStringLiteral("qrc:/mainw.ui.qml")));
    QObject *object = component.create(objectContext);
    return a.exec();
}
