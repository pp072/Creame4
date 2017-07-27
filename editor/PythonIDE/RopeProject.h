#ifndef ROPEPROJECT_H
#define ROPEPROJECT_H
#include <QObject>
#include <QVariant>
#include <QEventLoop>
#include <QEvent>
#include <QKeyEvent>
#include <QProcess>
#include <QAbstractSocket>
#include "IRopeProject.h"

#include "PythonQt.h"
#include "PythonQt_QtAll.h"
#include "gui/PythonQtScriptingConsole.h"
//#include "creameDLL.h"
#define QS_LOG_LINE_NUMBERS
#include "QsLog/QsLog.h"
#include "QsLog/QsLogDest.h"
#include <QSettings>
class rope_thread;
class QDomNode;
class QTcpSocket;

namespace Plugins
{
	class RopeFile;
	class RopeProject: public IRopeProject
	{
		Q_OBJECT
		friend class RopeFile;
	public:
		RopeProject();

		QString loadStartingScript( QString starting_script );

		void start_new_rope_server();
		virtual ~RopeProject();
	public:
		virtual IRopeFile* openFile(const QString & fileName);
		virtual void closeFile(const QString & fileName);
		virtual void revalidate(const QString & path, const QString & mainScript, const QString& projectPath);
		void makeRequest(const QString & methodName, const QVariantList & params, QObject * reciever = NULL, const char * slot = NULL);
		bool is_rope_running();
		void stop_rope();
		QHash<QString, RopeFile*> get_rope_files() const { return _files; }
	private:
		virtual void addSysPaths();
	public slots:
		void initProject(const QString & path, const QString & mainScript, const QString& projectPath);
		virtual void classNames(const QString & prefix);
	private slots:
		void readyError();
		void onProjectInited();
		void onProjectRevalidated();
		void onRunError(QProcess::ProcessError);
		void onReadyRead();

		void onClassList();
		void onError(QAbstractSocket::SocketError err);
		void stdOut(const QString& s);
		void stdErr(const QString& s);
	signals:
		void classListComplete(const QStringList & list);
		void serverStarted();
	private slots:
		//Ќужен дл€ виртуального нажати€ Enter в консоли
		void consoleVirtualPressEnter();
	private:
		QHash<QString, RopeFile*> _files;
		int _port;
		QEventLoop _loop;
		rope_thread* _rope_threadt;
		QString _stdErr;
		QString _stdOut;
		QProcess *_proc;
			
	public:	
		PythonQtScriptingConsole *_console;
		mutable QString _ip;
		PythonQtObjectPtr  _main_python_module;
		/*PythonQtObjectPtr getMain_python_context() const { return main_python_context; }*/
		QSettings *_PythonEditorSettings;	
		QString rope_module_path;
		QString rope_for_modules_path;
			
		//LOGGING
	public:
			
	};
	//¬спомогательный класс дл€ отлавливани€ нажати€ клавишь в консоли, чтобы остановить rope_server
	class KeyPressEaterForConsole : public QObject
	{
		Q_OBJECT
	private:
		RopeProject *_ropeProject;
	public:
		KeyPressEaterForConsole(RopeProject *ropeProj) : QObject(ropeProj){
			_ropeProject = ropeProj;
		};
	protected:
		bool eventFilter(QObject *obj, QEvent *event);
		void loggingPyConsoleExecuteLine();
	};
}
#endif // ROPEPROJECT_H
