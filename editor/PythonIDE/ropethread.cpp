#include "ropethread.h"
#include "RopeProject.h"
#include "RopeFile.h"

void rope_thread::run()
{
	QLOG_INFO() << " rope_thread::run() START";
	main_python_context = PythonQt::self()->getMainModule();
	QLOG_INFO() << " evalFile("<< _path <<")";
	main_python_context.evalFile(_path);
	QLOG_INFO() << " rope_thread::run() FINISH";
}

void rope_thread::setFileName( QString path )
{
	_path = path;
}