#include "mainwindow.h"
#include <QApplication>
#include <QLocale>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 設定應用程式的地區為繁體中文
    QLocale::setDefault(QLocale::Chinese);

    MainWindow w;
    w.show();
    return a.exec();
}
