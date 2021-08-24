#include "..\QtRMV2Widget\QtRMV2Widget.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QRMV2Widget w;
    w.show();
    return a.exec();
}
