#include "playerlogic.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PlayerLogic p;
    p.showMainWidget();

    return a.exec();
}
