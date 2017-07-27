#include <QDebug>
#include <QProcess>
#include <QApplication>
#include <QVariant>
#include <QTcpSocket>
#include <QStringBuilder>
#include <QMessageBox>
#include <QSettings>
#include "RopeProject.h"
#include "RopeFile.h"
#include "XMLRPC.h"
#include <QtGui>
#include <QDir>
#include "def_settings.h"
#define QSLOG_PATH "logs\\pythonIDE_log.txt"
#define QSLOG_SIZE 32768



#include "ropethread.h"
#define stop __asm nop
namespace Plugins
{

RopeProject::RopeProject():
    _ip(""),
    _port(9999),
	_proc(NULL)
{
	_rope_threadt = 0x0;
	stop
    //_proc = new QProcess();
    //connect(_proc, SIGNAL(readyReadStandardError()), SLOT(readyError()));

	//PythonQt::init( PythonQt::RedirectStdOut);
	

 	QsLogging::Logger& logger = QsLogging::Logger::instance();
 	logger.setLoggingLevel(QsLogging::TraceLevel);
 
	QsLogging::DestinationPtr fileDestination(
 		QsLogging::DestinationFactory::MakeFileDestination(QDir(QApplication::applicationDirPath()).filePath(QSLOG_PATH), true, QSLOG_SIZE, 2) );
 	QsLogging::DestinationPtr debugDestination(
 		QsLogging::DestinationFactory::MakeDebugOutputDestination() );
 	logger.addDestination(debugDestination);
 	logger.addDestination(fileDestination);
	QLOG_INFO() << "---------------START LOG-------------------";
	QLOG_INFO() << "Built with Qt" << QT_VERSION_STR << "running on" << qVersion();

	_PythonEditorSettings = new QSettings(SETTINGS_SCRIPT_EDITOR_FILE,	QSettings::IniFormat ,this);
	QLOG_INFO() << "Loaded settings";

	PythonQt::init(PythonQt::IgnoreSiteModule | PythonQt::RedirectStdOut);
	PythonQt_QtAll::init();

	
	_main_python_module = PythonQt::self()->getMainModule();
	addSysPaths();

	QString starting_script = loadStartingScript(SET_STARTING_SCRIPT);
	_main_python_module.evalScript(starting_script);
	_console = new PythonQtScriptingConsole(0,_main_python_module);
	KeyPressEaterForConsole *keyPressEater = new KeyPressEaterForConsole(this);
	_console->installEventFilter(keyPressEater);

	connect(PythonQt::self(), SIGNAL(pythonStdOut(const QString&)), this, SLOT(stdOut(const QString&)));
	connect(PythonQt::self(), SIGNAL(pythonStdErr(const QString&)), this, SLOT(stdErr(const QString&)));
	_console->show();

    rope_module_path = QApplication::applicationDirPath() % "/RopeServer/RopeServ.py";
    if (!QFile::exists(rope_module_path ))
	{
		QLOG_WARN() << "!QFile::exists(rope_module_path )";
        rope_module_path  = "/RopeServer/RopeServ.py";
	}
	start_new_rope_server();
	QLOG_INFO() << "RopeProject STARTED";
}

QString RopeProject::loadStartingScript( QString script_file_path )
{
	QLOG_INFO() << "Loading starting script...";
	QString script;
	QFile starting_script_file(_PythonEditorSettings->value(script_file_path).toString());
	if (starting_script_file.open(QIODevice::ReadOnly))
	{
		QTextStream in(&starting_script_file);
		QStringList temp;
		while (!in.atEnd()) 
		{
			QString line = in.readLine();
			//Если встречаются табы, то заменяем их на пробелы
			temp = line.split("\t");
			for (int i = 0; i < temp.size(); i++) 
			{
				if (temp[i] == "")
					script += "    ";
				else
					script +=temp[i]+"\n";
			}
		}
	}
	starting_script_file.close();	
	return script;
}

void RopeProject::addSysPaths()
{
	QStringList sysPaths;
	sysPaths.append(_PythonEditorSettings->value(SET_PYD_LIBS_PATH).toString());
	sysPaths.append( QCoreApplication::applicationDirPath());
	sysPaths.append(_PythonEditorSettings->value(SET_PY_LIBS_PATH).toString());
	sysPaths.append(_PythonEditorSettings->value(SET_SITE_PACKAGES_PATH).toString());
	//перезаписываем для того чтобы не подгружались библиотеки из python который установлен по умолчанию
	PythonQt::self()->overwriteSysPath(sysPaths);
}

void RopeProject::stdOut( const QString& s )
{
	QStringList out = QString(s).split(' ');

	_stdOut += s;
	int idx;
	while ((idx = _stdOut.indexOf('\n'))!=-1) {
		QLOG_INFO() << "PYTHON_CONSOLE stdOut: "<< _stdOut.left(idx);
		_stdOut = _stdOut.mid(idx+1);
	}

	if (out.length() == 4)
	{
		if(out[0]=="ropeserstart")
		{
			_ip = out[1];
			_port = out[2].toInt();

			initProject(rope_for_modules_path,"",rope_for_modules_path+"/proj.cfg");
		}
	}
	if (out.length() == 1)
	{
		if(out[0]=="ropeserstpd\n")
		{
			_ip = "";
			_port = 0;
			//виртуальное нажатие Enter в консоли
			QTimer::singleShot(0,this, SLOT(consoleVirtualPressEnter()));
		}
	}
}

void RopeProject::consoleVirtualPressEnter()
{
	QKeyEvent key(QEvent::KeyPress,Qt::Key_Return, Qt::NoModifier);
	_console->keyPressEvent(&key);
}

void RopeProject::stdErr( const QString& s )
{
	_stdErr += s;
	int idx;
	while ((idx = _stdErr.indexOf('\n'))!=-1) {
		QLOG_WARN() << "PYTHON_CONSOLE stdErr: "<< _stdErr.left(idx);
		_stdErr = _stdErr.mid(idx+1);
	}
}
RopeProject::~RopeProject()
{
	if(is_rope_running())
		stop_rope();
	if(_rope_threadt!=0x0)
	{
		delete _rope_threadt;
		_rope_threadt = 0x0;
	}
	delete _console;

	QHashIterator<QString, RopeFile*> i(_files);
	while (i.hasNext()) {
		i.next();
		delete i.value();	
	}
}

void RopeProject::onRunError(QProcess::ProcessError err)
{
    if (err == QProcess::FailedToStart){
		QLOG_ERROR() << "Error while run of python interpreter ";
        QMessageBox::critical(NULL, tr("Python error"), tr("Error while run of python interpreter"));
		_proc->terminate();
    }
}

IRopeFile* RopeProject::openFile(const QString & fileName)
{
	bool is_alrady_file_opens = false;
	QHashIterator<QString, RopeFile*> i(_files);
	while (i.hasNext()) {
		i.next();
		if(i.key() == fileName)
		{
			is_alrady_file_opens = true;
			break;
		}
	}
	if(!is_alrady_file_opens)
		_files[fileName] = new RopeFile(fileName, this);

	QString abs_path = QDir().absoluteFilePath(fileName);
	QStringList parts = abs_path.split("/");
	parts.removeAt(parts.size()-1);
	this->rope_for_modules_path = parts.join("/");

    return _files[fileName];
}

void RopeProject::closeFile(const QString & fileName)
{
    if (_files.contains(fileName)){
        delete _files.take(fileName);
    }
}

void RopeProject::revalidate(const QString & path, const QString & mainScript, const QString& projectPath)
{
    if (_ip.isEmpty())
        return;

    QSettings set;
    QVariantList list;
    list << path << mainScript << projectPath << QFileInfo(set.fileName()).absoluteDir().path()+"/classes";
    makeRequest("revalidate", list, this, SLOT(onProjectRevalidated()));
    _loop.exec();
}

void RopeProject::readyError()
{
   qDebug() << "ERR:" << _proc->readAllStandardError();
}

void RopeProject::initProject(const QString & path, const QString & mainScript, const QString& projectPath)
{
    if (_ip.isEmpty())
        return;

    QSettings set;
    QVariantList list;
    list << path << mainScript << projectPath << QFileInfo(set.fileName()).absoluteDir().path()+"/classes";
    makeRequest("initProject", list, this, SLOT(onProjectInited()));
    _loop.exec();
}

void RopeProject::onProjectInited()
{
    XMLRPC::readSocket(qobject_cast<QTcpSocket*>(sender()));
	emit serverStarted();
    _loop.exit();
}

void RopeProject::onProjectRevalidated()
{
    XMLRPC::readSocket(qobject_cast<QTcpSocket*>(sender()));
    _loop.exit();
    foreach(RopeFile * file, _files.values()){
        file->errorsList();
    }
}

void RopeProject::makeRequest(const QString & methodName, const QVariantList & params, QObject * reciever, const char * slot)
{
    if (_ip.isEmpty())
        return;
    if (_port == 9999)
        return;

    QTcpSocket *soc = new QTcpSocket(this);
    soc->connectToHost(_ip, _port);
    soc->waitForConnected();
    soc->setProperty("method", methodName);
    connect(soc, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(onError(QAbstractSocket::SocketError)));
    if (reciever && slot){
        connect(soc, SIGNAL(readyRead()), reciever, slot);
    }
    soc->write(XMLRPC::prepareRequest(methodName, params).toUtf8());
}

void RopeProject::classNames(const QString & prefix)
{
    makeRequest("classNames", QVariantList() << prefix, this, SLOT(onClassList()));
}

void RopeProject::onClassList()
{
    QStringList list = XMLRPC::parseResponce<QStringList>(XMLRPC::readSocket(qobject_cast<QTcpSocket*>(sender())));
    emit classListComplete(list);
}

void RopeProject::onError(QAbstractSocket::SocketError err)
{
    QTcpSocket * soc = qobject_cast<QTcpSocket*>(sender());
	QLOG_WARN() << "Error:" << soc->errorString() << soc->property("method");
    //qDebug() << "Error:" << soc->errorString() << soc->property("method");
}

void RopeProject::start_new_rope_server()
{
	bool is_use_exterla_python =  _PythonEditorSettings->value(SET_USE_EXTERNAL_PYTHON).toBool();
	QLOG_INFO() << "SET_USE_EXTERNAL_PYTHON :" << is_use_exterla_python ;

	if ( is_use_exterla_python )
	{
		if (!QFile::exists(rope_module_path))
			QLOG_ERROR() << "Can't find rope module in path: "<<rope_module_path;

		QString python_path =_PythonEditorSettings->value(SET_PYTHON_EXE).toString();
		if (!QFile::exists(python_path))
			QLOG_ERROR() << "Can't find python.exe module in path: "<< python_path;

		_proc = new QProcess();
		connect(_proc, SIGNAL(readyReadStandardError()), SLOT(readyError()));
		connect(_proc, SIGNAL(error(QProcess::ProcessError)), SLOT(onRunError(QProcess::ProcessError)));
		QStringList env = QProcess::systemEnvironment();
		env << _PythonEditorSettings->value(SET_SITE_PACKAGES_PATH).toString();
		env << _PythonEditorSettings->value(SET_PY_LIBS_PATH).toString();
		QString app_path = QCoreApplication::applicationDirPath();
		env << app_path;
		env << _PythonEditorSettings->value(SET_PYD_LIBS_PATH).toString();
		_proc->setEnvironment(env);
		_proc->start(python_path, QStringList() << rope_module_path);

		_proc->waitForStarted(1000);
		if (_proc->state() == QProcess::Running)
		{
			_proc->waitForReadyRead();
			QString p_stdout = _proc->readAllStandardOutput();
			if(rope_for_modules_path.isEmpty())
			{
				QString abs_path = QDir().absoluteFilePath(_PythonEditorSettings->value(SET_FILE_DEFAULT).toString());
				QStringList parts = abs_path.split("/");
				parts.removeAt(parts.size()-1);
				rope_for_modules_path = parts.join("/");
			}
			initProject(rope_for_modules_path,"",rope_for_modules_path+"/proj.cfg");
			stdOut(p_stdout);
			stop
		} else 
		{
			delete _proc;
			_proc = NULL;
		}
	}
	else
	{
		if(_rope_threadt!=0x0)
		{
			delete _rope_threadt;
			_rope_threadt = 0x0;
		}
		_rope_threadt = new rope_thread();
		_rope_threadt->setFileName(rope_module_path);

		
		QString path = QFile(rope_module_path).fileName();
		QString dir = path.section("/",0,-2);
		PythonQt::self()->addSysPath(dir);
		_rope_threadt->start();
	}
}

void RopeProject::onReadyRead()
{
	_proc->readAllStandardOutput();
	stop
}

bool RopeProject::is_rope_running()
{
		return !_ip.isEmpty();
}

void RopeProject::stop_rope()
{
	makeRequest("stopServ", QVariantList(), this, SLOT(rope_stoped()));
	if(_proc)
	{
		_proc->kill();
		_proc->waitForFinished();
		delete _proc;
	}
	
}

//************************************
// Plugins::KeyPressEaterForConsole
//************************************
bool KeyPressEaterForConsole::eventFilter( QObject *obj, QEvent *event )
{
	if (event->type() == QEvent::KeyPress) {
		if(_ropeProject->is_rope_running())
			_ropeProject->stop_rope();
		else
		{
			//реализуется метод для логирования введеных в консоль команд пользователя
			QKeyEvent *keyEvent = (QKeyEvent*)event;
			if (keyEvent->key() ==Qt:: Key_Return)
				loggingPyConsoleExecuteLine();
			QObject::eventFilter(obj, event);
		}
	} else {
		return QObject::eventFilter(obj, event);
	}
}

void KeyPressEaterForConsole::loggingPyConsoleExecuteLine()
{
	QString code = _ropeProject->_console->getConsoleText();
	QLOG_INFO() << "PYTHON_CONSOLE executeLine: "<< code;
}


}
