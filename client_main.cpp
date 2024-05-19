#include "client_main_window.h"
#include <QApplication>
#include <iostream>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    client_main_window Main_window;
    Main_window.show();

    return app.exec();
}
