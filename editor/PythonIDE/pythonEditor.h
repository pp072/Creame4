#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#ifdef DLL_NONCLIENT_BUILD // в директивах препроцессора проекта с dll нужно прописать DLL_NONCLIENT_BUILD
#   define DLL_EXPORT __declspec(dllexport)
#else
#   define DLL_EXPORT __declspec(dllimport)
#endif
#include <QMainWindow>
#include "GeneratedFiles/ui_pythonwriter.h"
#include <QtGui>
#include "Designer/designer.h"
/*#include <vld.h>*/
#include "SettingUI.h"
class QAction;
class QMenu;
class PythonTextEditor;
class FilesTabWidget;
class PythonQtObjectPtr;
//class SettingUI;
namespace Plugins
{
	class RopeFile;
	class IRopeFile;
	class RopeProject;
	struct CompletitionList;
}
class DLL_EXPORT PythonWriter : public QMainWindow
{
	Q_OBJECT
public:
	PythonWriter();

	

	~PythonWriter();
	QString curFile_abs_path();
	QString curFile_name();
	QSharedPointer<Plugins::RopeProject> _rope_proj;
	SettingUI* _settingUI;
	Plugins::IRopeFile* _file;
	FilesTabWidget * _files_tab_widget;
	
	QStringList onRunCodeFlushing;
	void flush_codes();
	QFileSystemModel _dir_model;
	QTreeView _dir_tree_view;
	PythonQtObjectPtr getMain_python_context() const;
protected:
	virtual void closeEvent(QCloseEvent *event);
	virtual void mousePressEvent(QMouseEvent *e);
private slots:
	void newFile();

	

	void open();
	bool save();
	bool saveAs();
	void about();
	void openPreferences();
	void documentWasModified();
	void onCodeComplite();
	void onRunCode();
	void onRunImmediatlyCode();
	void showDesigner();
	void rope_stop();
	void slot_cur_tab_sel_changed(int index);
	void slot_cur_tab_close(int index);
	void slot_dir_tree_clicked(QModelIndex index);

	void loadFile_UI( QString openning_file );

	void onAutoComplite(const Plugins::CompletitionList & list);
	void rope_stoped();
	void findImpl(QString str, int pos);
	void onFindImplementationComplite(const QString &file, int offset);
	
private:
	bool tryCreateNewFile(QString newName, QString exten, int number = 0);

	void createActions();
	void create_text_edit_conn();

	void createMenus();
	void createToolBars();
	void createStatusBar();
	void loadSettingsFile();
	void readSettings();
	void writeSettings();
	bool mayBeSave();
	void loadFile(const QString &fileName);

	bool is_file_already_open();

	bool saveFile(const QString &fileName);
	void setCurrentFile(const QString &fileName);
	QString strippedName(const QString &fullFileName);
	QString getType(const QString & named);
	
	QEventLoop _loop;
	Designer* _designer;
	PythonTextEditor* _text_edit;
	QString curFile;
	//QSettings *_settings;
	QSettings *_PythonEditorSettings;
	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *editCodeRunMenu;
	QMenu *helpMenu;
	QToolBar *fileToolBar;
	QToolBar *editToolBar;
	QToolBar *editCodeRunBar;
	QAction *run_code;
	QAction *action_designer;
	QAction *run_code_im;
	QAction *newAct;
	QAction *openAct;
	QAction *saveAct;
	QAction *saveAsAct;
	QAction *exitAct;
	QAction *cutAct;
	QAction *copyAct;
	QAction *pasteAct;
	QAction *aboutAct;
	QAction *aboutQtAct;
	QAction *prefAct;
private:
	Ui::PythonWriter ui;
};

#endif
