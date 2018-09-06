#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("xalanq");
    QCoreApplication::setApplicationName("Week2 Assignment");

    MainWindow w;
    w.show();

    return a.exec();
}
