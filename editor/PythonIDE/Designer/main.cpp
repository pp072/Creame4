#include "designer.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Designer w;
	w.show();
	return a.exec();
}
