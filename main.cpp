#include "deskping.h"
#include "deskwatch.h"

#include <QApplication>
#include <synchapi.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DeskPing w;
    w.show();

    DeskWatch W;
    W.show();

    return a.exec();
}
