#include "editor.h"
#include "Horde3DUtils.h"

#include "propertyeditor.h"
#include "def_settings.h"
#include "PythonQt.h"
#include "treeViewNodesForm.h"
#include "SceneWizard.h"
#include "SceneFile.h"
#include "QReferenceNode.h"
#include "QModelNode.h"
#include "QLightNode.h"
#include "QGroupNode.h"
#include "QMeshNode.h"
#include "QJointNode.h"
#include "QEmitterNode.h"
#include "QCameraNode.h"
#include "SceneTreeModel.h"
#include "GameObject.h"


#include "game.h"
#define stop __asm nop
editor::editor(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags),m_sceneFile(0x0)
{
	ui.setupUi(this);
	setCentralWidget(0);
	setDockNestingEnabled(true);
	_main_window_handle = this;

	PythonQt::init(PythonQt::IgnoreSiteModule | PythonQt::RedirectStdOut);
	PythonQtObjectPtr mainModule = PythonQt::self()->getMainModule();
	PythonQt::self()->registerCPPClass("GameObject", "","Game", PythonQtCreateObject<GameObjectWrapper>);
	PythonQt::self()->registerCPPClass("EngineHRD", "","Game", PythonQtCreateObject<EngineHRDWrapper>);
	
	_treeViewForm = new TreeViewForm(this);
 	PythonQt::self()->registerClass(&TreeViewForm::staticMetaObject, "GUIModule", PythonQtCreateObject<PythonWrapper_TreeViewForm>);
 	mainModule.addObject("tree_nodes_view", _treeViewForm);
 	mainModule.evalScript("from PythonQt import *\nGUIModule.TreeViewFormGL = tree_nodes_view;");

	PythonQt::self()->registerClass(&editor::staticMetaObject, "GUIModule", PythonQtCreateObject<editorPythonWrapper>);
	mainModule.addObject("_editor", this);
	mainModule.evalScript("from PythonQt import *\nGUIModule.Editor = _editor;");



	_pythonWriter = new PythonWriter();
	ui.python_dockWidget->setWidget(_pythonWriter);

	_propertyEditor = new PropertyEditor(this);

	_rcWinMan = new RCWinMan(this);
	_rcWinMan->getPrimeWindow()->_dockWidget = ui.dockWidget_2;
	ui.dockWidget_2->setWidget((QWidget*)_rcWinMan->getPrimeWindow());
	loadScene();
	_rcWinMan->getPrimeWindow()->setCamera(2);

 	

	ui.NodesTree_dockWidget->setWidget(_treeViewForm);
	ui.Property_dockWidget->setWidget(_propertyEditor);
	_treeViewForm->full_update_tree_from_engine();
	connect(_treeViewForm, SIGNAL(signal_selectionChanged(unsigned int)), _propertyEditor, SLOT(slot_get_item_properties(unsigned int)));
	connect(_treeViewForm, SIGNAL(signal_selectionChanged(unsigned int)), _rcWinMan, SLOT(slot_get_item_selected(unsigned int)));

	_renderTimer = new QTimer(this);
	_renderTimer->start(1000.0/60.0);
 	connect(_renderTimer, SIGNAL(timeout()),_rcWinMan, SLOT(render()));


	_instruments = new InstrumentsManager(this);
	connect(_rcWinMan->getPrimeWindow()->getEngineGrView(),SIGNAL(signal_emit_RenderReadyToOverlays(EngineGrView*)),
		_instruments,SLOT(render_instruments(EngineGrView*)));
	

 	connect(_rcWinMan->getPrimeWindow()->getEngineGrView(),SIGNAL(signal_emit_RenderReadyToOverlays(EngineGrView*)),
 		this,SLOT(canLoadingNodes()));
	connect (_rcWinMan->getPrimeWindow()->getEngineGrView(),SIGNAL(signal_emit_RenderBefore(EngineGrView*, float)),
		this, SLOT(update_mans(EngineGrView*, float)));

	connect(ui.actionAddWindow,SIGNAL(triggered(bool)),this,SLOT(addRCWindow(bool)));
	connect(ui.actionNewScene,SIGNAL(triggered(bool)),this,SLOT(newScene(bool)));
	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(openScene()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(saveSceneAs()));
	connect(ui.actionTest, SIGNAL(triggered()), this, SLOT(actionTest()));
	connect(ui.actionRun_game, SIGNAL(triggered(bool)), this, SLOT(runGame(bool)));
	read_window_settings();

	_log_window = new log_window(this);
	ui.m_logWidgetDock->setWidget(_log_window);
	connect(_rcWinMan,SIGNAL(logMessages(const QList<QListWidgetItem*>&)),_log_window,SLOT(updateLog(const QList<QListWidgetItem*>&)));
	//connect(_rcWinMan,SIGNAL(logMessages(const QList<QListWidgetItem*>&)),_log_window,SLOT(updateLog(const QList<QListWidgetItem*>&)));

	m_pluginManager = new PlugInManager(this);
	connect(m_pluginManager, SIGNAL(nodeRegistered(const QString&, PlugInManager::NodeType)), this, SLOT(nodeRegistered(const QString&, PlugInManager::NodeType)));
	connect(m_pluginManager, SIGNAL(nodeUnregistered(const QString&, PlugInManager::NodeType)), this, SLOT(nodeUnregistered(const QString&, PlugInManager::NodeType)));
	connect(m_pluginManager, SIGNAL(toolRegistered(const QList<QAction*>&)), this, SLOT(toolRegistered(const QList<QAction*>&)));
	m_pluginManager->setPropertyWidget(ui.m_nodeWidget);
	m_pluginManager->init();

	m_pluginManager->registerSceneNode("Reference", QReferenceNode::loadNode, SceneWizard::createReference);
	m_pluginManager->registerSceneNode("Model", QModelNode::loadNode, 0);
	m_pluginManager->registerSceneNode("Mesh", QMeshNode::loadNode, 0);
	m_pluginManager->registerSceneNode("Light", QLightNode::loadNode, SceneWizard::createLight);
	m_pluginManager->registerSceneNode("Group", QGroupNode::loadNode, 0);
	m_pluginManager->registerSceneNode("Joint", QJointNode::loadNode, 0);
	m_pluginManager->registerSceneNode("Emitter", QEmitterNode::loadNode, 0);
	m_pluginManager->registerSceneNode("Camera", QCameraNode::loadNode, 0);


	connect(ui.m_sceneTreeWidget, SIGNAL(currentNodeChanged(QXmlTreeNode*)), ui.m_nodeWidget, SLOT(setCurrentNode(QXmlTreeNode*)));

	ui.m_sceneTreeWidget->m_sceneTreeView->addAction(ui.m_actionCopy);
	ui.m_sceneTreeWidget->m_sceneTreeView->addAction(ui.m_actionCut);
	ui.m_sceneTreeWidget->m_sceneTreeView->addAction(ui.m_actionPaste);
	ui.m_sceneTreeWidget->m_sceneTreeView->addAction(ui.m_actionRemove);
	connect(ui.m_actionCut, SIGNAL(triggered()), ui.m_sceneTreeWidget, SLOT(cutCurrentNode()));
	connect(ui.m_actionCopy, SIGNAL(triggered()), ui.m_sceneTreeWidget, SLOT(copyCurrentNode()));
	connect(ui.m_actionPaste, SIGNAL(triggered()), ui.m_sceneTreeWidget, SLOT(pasteNode()));
	connect(ui.m_actionRemove, SIGNAL(triggered()), ui.m_sceneTreeWidget, SLOT(removeCurrentNode()));

	// create the undo group and connect different undo stacks with it
	m_undoGroup = new QUndoGroup(this);
	connect(m_undoGroup, SIGNAL(canUndoChanged(bool)), this, SLOT(setModified()));
	QAction* redoAction = m_undoGroup->createRedoAction(m_undoGroup);
	redoAction->setIcon(QIcon(":/editor/icons/Redo.png"));
	redoAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Y));
	ui.menuEdit->insertAction(ui.menuEdit->actions().isEmpty() ? 0 : ui.menuEdit->actions().first(), redoAction);		
	QAction*  undoAction = m_undoGroup->createUndoAction(m_undoGroup);
	undoAction->setIcon(QIcon(":/editor/icons/Undo.png"));
	undoAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Z));
	ui.menuEdit->insertAction(redoAction, undoAction);
	ui.mainToolBar->addAction(undoAction);
	ui.mainToolBar->addAction(redoAction);
	ui.mainToolBar->addSeparator();
	connect(ui.m_sceneTreeWidget, SIGNAL(undoStackActivated(QUndoStack*)), this, SLOT(setUndoStack(QUndoStack*)));
	connect(ui.m_sceneTreeWidget, SIGNAL(signal_selectionChanged(unsigned int)), _rcWinMan, SLOT(slot_get_item_selected(unsigned int)));
	connect(_instruments->getInstr_select_object_by_mouse(), SIGNAL(transformChanged(const float, const float, const float, const float, const float, const float, const float, const float, const float)),
											  ui.m_nodeWidget, SLOT(transformChanged(const float, const float, const float, const float, const float, const float, const float, const float, const float)));


	_game = new Game(this);
	//connect(ui.m_sceneTreeWidget, SIGNAL(currentNodeChanged(QXmlTreeNode*)), m_pluginManager, SLOT(setCurrentNode(QXmlTreeNode*)));
	//connect(ui.m_sceneTreeWidget, SIGNAL(currentNodeChanged(QXmlTreeNode*)), m_nodeWidget, SLOT(setCurrentNode(QXmlTreeNode*)));
	//connect(ui.m_sceneTreeWidget, SIGNAL(currentNodeChanged(QXmlTreeNode*)), this, SLOT(updateNodeActions(QXmlTreeNode*)));

	//connect(ui.m_sceneTreeWidget, SIGNAL(nodePropertyChanged(QXmlTreeNode*)), m_cameraToolBar, SLOT(nodePropertyChanged(QXmlTreeNode*)));
	//connect(ui.m_sceneTreeWidget, SIGNAL(nodePropertyChanged(QXmlTreeNode*)), m_nodeWidget, SLOT(setCurrentNode(QXmlTreeNode*)));
	// Actions may depend on the dynamic properties
	//connect(ui.m_sceneTreeWidget, SIGNAL(nodePropertyChanged(QXmlTreeNode*)), this, SLOT(updateNodeActions(QXmlTreeNode*)));

	//m_sceneTreeWidget->m_sceneTreeView->addAction(m_actionAddAttachment);
	//connect(m_actionAddAttachment, SIGNAL(triggered()), m_sceneTreeWidget->m_sceneTreeView, SLOT(addAttachmentNode()));
	
}

editor::~editor()
{
	write_window_settings();

}
void editor::setUndoStack(QUndoStack* undoStack)
{
	m_undoGroup->addStack(undoStack);	
	m_undoGroup->setActiveStack(undoStack);
}
void editor::nodeRegistered(const QString& name, PlugInManager::NodeType type)
{
	QAction* action = new QAction(tr("Add %1").arg(name), this);
	action->setData(name);
	if(type == PlugInManager::SCENENODE)
	{
		if(name == "Reference")
			action->setIcon(QIcon(":/tree/icons/3D_cube1.png"));
 		connect(action, SIGNAL(triggered()), this, SLOT(addNode()));
// 		m_menuScene->insertAction(m_menuScene->actions().first(), action);
 		ui.m_sceneTreeWidget->m_sceneTreeView->addAction(action);
	}
}

void editor::nodeUnregistered(const QString& name, PlugInManager::NodeType /*type*/)
{
// 	QList<QAction*> actions = m_menuScene->actions();
// 	actions << m_menuExtras->actions();
// 	foreach(QAction* action, actions)
// 	{
// 		if (action->text() == tr("Add %1").arg(name))
// 		{
// 			delete action;
// 			break;
// 		}
// 	}
}
void editor::addRCWindow( bool )
{
	QDockWidget *dockWidget = new QDockWidget(this);

	RCWindow *win = _rcWinMan->createNewWindow();
	win->_dockWidget = dockWidget;
	dockWidget->setWidget((QWidget*)win);
	this->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget);

	connect(win->getEngineGrView(),SIGNAL(signal_emit_RenderReadyToOverlays(EngineGrView*)),
		_instruments,SLOT(render_instruments(EngineGrView*)));

	win->show();
	win->setCamera(2);
}

void editor::newScene(bool isOn)
{
	m_sceneFile = SceneWizard::createScene(this);
	if(!m_sceneFile)
		return;
	if (m_sceneFile) m_sceneFile = SceneFile::load(m_sceneFile->absoluteSceneFilePath()); // load the new scene
	
//	_treeViewForm->full_update_tree_from_file(m_sceneFile);
	if (!closeScene())
		return; // if closing was canceled return
	sceneCreated();
	

		
	stop
}
void editor::openScene( QString fileName /*= QString()*/ )
{
	// check if there has been a file specified 
	if (fileName.isEmpty()) // if not open a dialog
		fileName = QFileDialog::getOpenFileName(this, tr("Select scene file"), QDir::currentPath(), tr("Scene files (*.scn);;All files (*.*)"));
	// if user canceled the file dialog return
	if (fileName.isEmpty())
		return;	
	// try to close any existing scene
	if(!QFile::exists(fileName))
	{
		getLog_window()->updateLog(new QListWidgetItem(tr("File: ")+ fileName +(" isn't exists!"), 0, 1));
		return;
	}
	reset_mans();
	if (!closeScene())
		return; // if closing was canceled return
	// Open Config file where the pipeline.xml, scene directories and the scene graph file are stored
	m_sceneFile = SceneFile::load(fileName);

	connect(_rcWinMan->getPrimeWindow()->getEngineGrView(),SIGNAL(signal_emit_RenderReadyToOverlays(EngineGrView*)),
		this,SLOT(canLoadingScene()),Qt::DirectConnection);
	_rcWinMan->getPrimeWindow()->getEngineGrView()->viewport()->repaint();

	disconnect(_rcWinMan->getPrimeWindow()->getEngineGrView(),SIGNAL(signal_emit_RenderReadyToOverlays(EngineGrView*)),this,SLOT(canLoadingScene()));
}

void editor::canLoadingScene()
{
		sceneCreated();
}

void editor::canLoadingNodes()
{
	if(refs_xml_nodes.isEmpty()) 
		return;

	for (int i = 0; i < refs_xml_nodes.size(); ++i) 
	{
		QAction* source = refs_xml_nodes.at(i);
		refs_xml_nodes.pop_front();
		SceneTreeModel* treeModel = qobject_cast<SceneTreeModel*>(ui.m_sceneTreeWidget->m_sceneTreeView->model());
		QSceneNode* sceneNode = static_cast<QSceneNode*>(treeModel->rootNode());
		// If the slot was called from the context menu...
		if( ui.m_sceneTreeWidget->m_sceneTreeView->getContextCall() && ui.m_sceneTreeWidget->m_sceneTreeView->currentIndex().isValid() )
		 	// ... select the currently selected node as root
		 	sceneNode = static_cast<QSceneNode*>(ui.m_sceneTreeWidget->m_sceneTreeView->currentIndex().internalPointer());	
		QDomElement newNode = editor::instance()->pluginManager()->createNode( source->data().toString(), this );
		if( !newNode.isNull() )
			treeModel->undoStack()->push((QUndoCommand*)ui.m_sceneTreeWidget->m_sceneTreeView->createAddUndoCommand(newNode, sceneNode->xmlNode(), treeModel, source->text()));	
		
	}
}

void editor::addNode()
{
	//RefXmlNode *node = new RefXmlNode;
	QAction* source = qobject_cast<QAction*>(sender());
	if( !source ) return;
	refs_xml_nodes.push_back(source);
	
}

void editor::sceneCreated()
{
	
	setWindowTitle(m_sceneFile->absoluteSceneFilePath()+"[*]");
	m_sceneFile->init();
	_log_window->clear();
	ui.m_sceneTreeWidget->setScene(m_sceneFile);
	h3dutLoadResourcesFromDisk(".");
	_treeViewForm->full_update_tree_from_engine();
	//_treeViewForm->full_update_tree_from_engine();
}

void editor::saveSceneAs()
{
	Q_ASSERT ( !m_sceneFile->sceneFileName().isEmpty() );

	ui.m_sceneTreeWidget->save();
	if (!m_sceneFile->save())
		QMessageBox::warning(this, tr("Error"), tr("Error writing to file"));
	else
		setWindowModified(false);	
}
bool editor::closeScene()
{
	ui.m_sceneTreeWidget->setScene(0);
	m_pluginManager->setSceneFile(0);
	return true;
}

void editor::loadScene()
{
 	std::string _application_path =  qPrintable(QCoreApplication::applicationDirPath()); 
 	std::string _contentDir = _application_path + "/Content";

 	H3DRes _forwardPipeRes = h3dAddResource( H3DResTypes::Pipeline, "pipelines/editor.pipeline.xml", 0 );
	H3DNode _cam = h3dAddCameraNode( H3DRootNode, "editor_camera", _forwardPipeRes );
	h3dutLoadResourcesFromDisk( _contentDir.c_str() );
}

editor* editor::get_main_window_handle()
{
	return _main_window_handle;
}
void editor::write_window_settings()
{
	QSettings settings(SETTINGS_EDITOR_FILE,	QSettings::IniFormat);
	settings.setValue("editor/geometry", this->saveGeometry());
	settings.setValue("editor/windowState", this->saveState());
}

void editor::read_window_settings()
{
	QSettings settings(SETTINGS_EDITOR_FILE,	QSettings::IniFormat);
	this->restoreGeometry(settings.value("editor/geometry").toByteArray());
	this->restoreState(settings.value("editor/windowState").toByteArray());
}

PlugInManager* editor::pluginManager()
{
	return m_pluginManager;
}

editor* editor::instance()
{
	return _main_window_handle;
}

unsigned int editor::get_cur_node_selected()
{
	return _rcWinMan->getSelected_item();
}

void editor::actionTest()
{

}

void editor::update_mans( EngineGrView* egv, float _anim_times)
{
	GameAnimationManager::getInstance()->update(_anim_times);
}

void editor::reset_mans()
{
	GameAnimationManager::getInstance()->reset();
}

void editor::runGame(bool isOn)
{
	if(isOn)
	{
		_rcWinMan->getPrimeWindow()->getEngineGrView()->stop_render();
		connect(_rcWinMan->getPrimeWindow()->getEngineGrView(),SIGNAL(signal_emit_RenderReadyToOverlays(EngineGrView*)),
			_game,SLOT(initGame()),Qt::DirectConnection);
		_rcWinMan->getPrimeWindow()->getEngineGrView()->viewport()->repaint();
		disconnect(_rcWinMan->getPrimeWindow()->getEngineGrView(),SIGNAL(signal_emit_RenderReadyToOverlays(EngineGrView*)),_game,SLOT(initGame()));

		if(_rcWinMan->getPrimeWindow()->getCamera() != GameEngine::getActiveCamera())
			_rcWinMan->getPrimeWindow()->setCamera(GameEngine::getActiveCamera());
		_rcWinMan->getPrimeWindow()->getEngineGrView()->resize(_rcWinMan->getPrimeWindow()->getEngineGrView()->size().width(),_rcWinMan->getPrimeWindow()->getEngineGrView()->size().height()-1);
		_rcWinMan->getPrimeWindow()->getEngineGrView()->resize(_rcWinMan->getPrimeWindow()->getEngineGrView()->size().width(),_rcWinMan->getPrimeWindow()->getEngineGrView()->size().height()+1);

		connect(_rcWinMan->getPrimeWindow()->getEngineGrView(),SIGNAL(signal_emit_RenderReadyToOverlays(EngineGrView*)), this,SLOT(renderGame()));
	}else
	{
		disconnect(_rcWinMan->getPrimeWindow()->getEngineGrView(),SIGNAL(signal_emit_RenderReadyToOverlays(EngineGrView*)), this,SLOT(renderGame()));
		_rcWinMan->getPrimeWindow()->getEngineGrView()->start_render();
	}
}

void editor::renderGame()
{
	_game->update();
}
