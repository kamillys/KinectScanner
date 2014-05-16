#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //try {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();

        return a.exec();
    //} catch (...)
    //{
    //}
    return 0;
}
