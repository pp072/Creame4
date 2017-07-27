#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include "PythonQt.h"
class rope_thread : public QThread
{
	Q_OBJECT

public:
	void run();
	PythonQtObjectPtr  main_python_context;
	QString _ip;
	int _port;
	QString _path;
	void setFileName( QString path );
};

#endif // MYTHREAD_H
