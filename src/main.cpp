#include <QApplication>
#include "FeedWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    FeedWindow w;
    w.show();

    return a.exec();
}
