#include <QApplication>
#include "feedWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    FeedWindow w;
    w.show();

    return a.exec();
}
