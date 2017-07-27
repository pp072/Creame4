#ifndef EDITOR_H
#define EDITOR_H

#include <QtGui/QMainWindow>
#include "GeneratedFiles/ui_editor.h"
//nclude "enginegrview.h"
//#include "rcwindow.h"
#include "rcwinman.h"
#include "instruments.h"
#include "log_window.h"
#include "pythonEditor.h"
#include "PlugInManager.h"
class TreeViewForm;
class PropertyEditor;
class SceneFile;
static class editor *_main_window_handle;
class Game;


class editor : public QMainWindow
{
	Q_OBJECT
	//friend class PlugInManager;
	friend class editorPythonWrapper;
public:
	editor(QWidget *parent = 0, Qt::WFlags flags = 0);
	~editor();
	//EngineGrView			*_engineGrView1;
	static editor* instance();
	TreeViewForm			*_treeViewForm;
	PropertyEditor * getPropertyEditor() const {return _propertyEditor;};
	InstrumentsManager		*_instruments;
	QTimer					*_renderTimer;
	RCWinMan				*_rcWinMan;
	PythonWriter			*_pythonWriter;
	
	PlugInManager* pluginManager();
	log_window * getLog_window() const { return _log_window; }
	SceneFile * getSceneFile() const { return m_sceneFile; }
	Ui::editorClass ui;
public:
	static editor* get_main_window_handle();
	unsigned int get_cur_node_selected();
private:
	PropertyEditor			*_propertyEditor;
	log_window				*_log_window;
	PlugInManager			*m_pluginManager;
	SceneFile				*m_sceneFile;
	
	QUndoGroup				*m_undoGroup;
	QVector<QAction*> refs_xml_nodes;
	void write_window_settings();
	void read_window_settings();
	Game					*_game;
	
private slots:
	void addRCWindow(bool);
	void loadScene();
	void newScene(bool);
	void openScene(QString fileName = QString());
	void nodeRegistered(const QString& name, PlugInManager::NodeType type);
	void nodeUnregistered(const QString& name, PlugInManager::NodeType /*type*/);
	void sceneCreated();
	bool closeScene();
	void actionTest();
	void runGame(bool);
	void saveSceneAs();
	void canLoadingScene();
	void renderGame();
	void canLoadingNodes();
	void setUndoStack(QUndoStack* undoStack);
	void addNode();
	void update_mans(EngineGrView*, float);
	void reset_mans();

};
class editorPythonWrapper : public QObject
{
	Q_OBJECT
public slots:
	void openScene(editor *o,QString fileName){o->openScene(fileName);}
};
#endif // EDITOR_H
