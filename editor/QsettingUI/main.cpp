#include <QSettings>
#include <QtGui/QApplication>
#include "SettingUI.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SettingUI w;
    w.show();

    return a.exec();
}
