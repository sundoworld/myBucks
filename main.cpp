#include <QApplication>

#include "mybucks.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    myBucks *dialog = new myBucks;

    dialog->show();
    return app.exec();
}
