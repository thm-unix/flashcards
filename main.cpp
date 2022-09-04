#include "choosestack.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    choosestack w;
    w.show();
    return a.exec();
}
