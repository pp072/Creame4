#include "treeViewNodesForm.h"
//#include "creameDLL.h"
#include "Horde3D.h"
#include "Horde3DUtils.h"
#define stop __asm nop
TreeViewForm::TreeViewForm(QWidget *parent)
: QMainWindow(parent),_filter(type::Undefined)
{
	ui.setupUi(this);
	setup_model();
	_cur_node_selected = 1;
	connect(ui.actionInsert_child_row,SIGNAL(triggered()),this,SLOT(insert_child_row()));
	connect(ui.actionInsert_row,SIGNAL(triggered()),this,SLOT(insert_row()));
	connect(ui.actionDelete_row,SIGNAL(triggered()),this,SLOT(delete_row()));
	connect(ui.actionRefresh,SIGNAL(triggered()),this,SLOT(slot_full_update_tree()));
	connect(ui.actionFindNodeInTree,SIGNAL(triggered()),this,SLOT(slot_find_node_in_tree()));
	connect(ui.treeView->selectionModel(),
		SIGNAL(selectionChanged(const QItemSelection &,
		const QItemSelection &)),
		this, SLOT(update_actions()));

	_combo_box = new QComboBox(this);
	QLabel * label = new QLabel(this);
	label->setText("filter:");
	ui.toolBar->addWidget(label);
	
	_combo_box->insertItem(0,QIcon(""),"all");
	_combo_box->insertItem(1,QIcon(":/tree/icons/Group.png"),"group");
	_combo_box->insertItem(2,QIcon(":/tree/icons/3D_cube1.png"),"model");
	_combo_box->insertItem(3,QIcon(":/tree/icons/Cube.png"),"mesh");
	_combo_box->insertItem(4,QIcon(":/tree/icons/dog_bone.png"),"joint");
	_combo_box->insertItem(5,QIcon(":/tree/icons/Light.png"),"light");
	_combo_box->insertItem(6,QIcon(":/tree/icons/Cam.png"),"camera");
	_combo_box->insertItem(7,QIcon(":/tree/icons/Krugi_plotnosti_IC.png"),"emitter");
	
	
	

	ui.toolBar->addWidget(_combo_box);
	connect(_combo_box, SIGNAL(currentIndexChanged (int)), this, SLOT(apply_filter(int)));
// 	QSettings settings("windowSettings.ini",	QSettings::IniFormat);
// 	this->restoreGeometry(settings.value("treeViewForm/geometry").toByteArray());
// 	this->restoreState(settings.value("treeViewForm/windowState").toByteArray());
//	PythonQt::self()->registerClass(&TreeViewForm::staticMetaObject, "GUIModule", PythonQtCreateObject<PythonWrapper_TreeViewForm>);
}

void TreeViewForm::apply_filter( int item)
{
	_filter = (type)item;
	slot_full_update_tree();
}



TreeViewForm::~TreeViewForm()
{
// 	QSettings settings("windowSettings.ini",	QSettings::IniFormat);
// 	settings.setValue("treeViewForm/geometry", this->saveGeometry());
// 	settings.setValue("treeViewForm/windowState", this->saveState());
}

void TreeViewForm::setup_model()
{
	_model = 
		QSharedPointer<treeViewFormModel>(new treeViewFormModel(this),&QObject::deleteLater);

	_model->set_root_node();
	ui.treeView->setModel(_model.data());
	update_actions();
}

void TreeViewForm::insert_child_row()
{
	QModelIndex index = ui.treeView->selectionModel()->currentIndex();
	QAbstractItemModel *model = ui.treeView->model();

	if (!model->insertRow(0, index))
		return;

	update_actions();

	ui.treeView->selectionModel()->setCurrentIndex(model->index(0, 0, index),
		QItemSelectionModel::ClearAndSelect);
}

void TreeViewForm::insert_row()
{
	QModelIndex index = ui.treeView->selectionModel()->currentIndex();
	QAbstractItemModel *model = ui.treeView->model();

	if (!model->insertRow(index.row()+1, index.parent()))
		return;

	update_actions();
}

void TreeViewForm::delete_row()
{
	QModelIndex index = ui.treeView->selectionModel()->currentIndex();
	QAbstractItemModel *model = ui.treeView->model();

	if (model->removeRow(index.row(), index.parent()))
		update_actions();
}

void TreeViewForm::update_actions()
{
	bool hasSelection = !ui.treeView->selectionModel()->selection().isEmpty();
	ui.actionDelete_row->setEnabled(hasSelection);

	bool hasCurrent = ui.treeView->selectionModel()->currentIndex().isValid();
	ui.actionInsert_row->setEnabled(hasCurrent);

	if (hasCurrent) {
		ui.treeView->closePersistentEditor(ui.treeView->selectionModel()->currentIndex());

		int row = ui.treeView->selectionModel()->currentIndex().row();
		int column = ui.treeView->selectionModel()->currentIndex().column();
		if (ui.treeView->selectionModel()->currentIndex().parent().isValid())
			statusBar()->showMessage(tr("Position: (%1,%2)").arg(row).arg(column));
		else
			statusBar()->showMessage(tr("Position: (%1,%2) in top level").arg(row).arg(column));

		treeViewFormNode *node = static_cast<treeViewFormNode *> (ui.treeView->selectionModel()->currentIndex().internalPointer());

// 
// 		if(_cur_node_selected !=  node->get_handle())
// 		{
			_cur_node_selected = node->get_handle();
			emit signal_selectionChanged(_cur_node_selected);
//		}
	}
}

void TreeViewForm::update_nodes_from_engine(int RootNode, treeViewFormNode *parent_item )
{
	int childCounter=0;
	while(1)
	{
		H3DNode node=h3dGetNodeChild(RootNode,childCounter);
		if (!node)
			break;

		QString node_name = h3dGetNodeParamStr( node, H3DNodeParams::NameStr );


		
		QSharedPointer<treeViewFormNode> child_item = 
			QSharedPointer<treeViewFormNode>(new treeViewFormNode(parent_item, node_name, node), &QObject::deleteLater);
		child_item->set_type(h3dGetNodeType(node));



		child_item->_parent = QWeakPointer<treeViewFormNode>(parent_item);

		if(_filter != type::Undefined)
		{
			if(h3dGetNodeType(node) == _filter)
				parent_item->_childs.insert(0, child_item);
			update_nodes_from_engine(node,parent_item);
		}else
		{
			update_nodes_from_engine(node,child_item.data());
			parent_item->_childs.insert(0, child_item);
		}

		childCounter++;
	}
}

bool TreeViewForm::full_update_tree_from_file( SceneFile* sceneFile )
{

	if( sceneFile )
	{
		if (loadSceneGraph(sceneFile->sceneGraphFile()))
		{
			return true;
		}
		else return false;		
	}
	else
	{
		return true;
	}	
}

bool TreeViewForm::loadSceneGraph( QString fileName )
{
 	QDomDocument root;
 	root.setContent( QString( "<Reference/>" ) );
 	root.documentElement().setAttribute("name", fileName);	
 	root.documentElement().setAttribute("sceneGraph", QDir(h3dutGetResourcePath(H3DResTypes::SceneGraph)).relativeFilePath(fileName));
 	return true;
	//m_rootNode = new QReferenceNode(root.documentElement(), 0, 0, 0);
// 	if (!m_rootNode->model())
// 	{		
// 		delete m_rootNode;
// 		m_rootNode = 0;
// 		return false;
 }

void TreeViewForm::full_update_tree_from_engine()
{
	_model->set_root_node();
	treeViewFormNode *parent_item = _model->get_root_node().data();
	parent_item->set_name("root");
	parent_item->set_handle("handle");
	parent_item->set_type(Undefined);

	update_nodes_from_engine(H3DRootNode, parent_item);
	_model->reset_model();
}

void TreeViewForm::slot_full_update_tree()
{
	full_update_tree_from_engine();
}

void TreeViewForm::selectByHandle(unsigned int Handle )
{
	QModelIndex ind = ui.treeView->model()->index(-1,-1);
	int iMdlChidCound =  ui.treeView->model()->rowCount(ind);

	QModelIndexList children;
	for ( int i = 0; i < ui.treeView->model()->rowCount(); ++i ) {
		children << ui.treeView->model()->index( i, 0 );  //  Use whatever column you are interested in.
	}

	for ( int i = 0; i < children.size(); ++i ) {
		for ( int j = 0; j < ui.treeView->model()->rowCount( children[i] ); ++j ) {
			children << children[i].child( j, 0 );
		}
	}

 	QString handleqstr =  QString::number(Handle);

	int count = children.count();
	for(int i = 0; i < count; i++)
	{
		QModelIndexList ModelsList= ui.treeView->model()->match(ui.treeView->model()->index(0,1) , Qt::DisplayRole, handleqstr,-1,Qt::MatchStartsWith | Qt::MatchWrap | Qt::MatchRecursive);
		if (!ModelsList.count())
			ModelsList= ui.treeView->model()->match(ui.treeView->model()->index(0,1,children.at(i)) , Qt::DisplayRole, handleqstr,-1,Qt::MatchStartsWith | Qt::MatchWrap | Qt::MatchRecursive);
		if(ModelsList.count()>0)
	 	{
	 		QModelIndex model= ModelsList.at(0);
	 		treeViewFormNode *item = static_cast<treeViewFormNode *>(model.internalPointer());
	 
	 		ui.treeView->clearSelection();
	 		ui.treeView->selectionModel()->select( model,QItemSelectionModel::Select | QItemSelectionModel::Rows);

 			if(Handle == item->get_handle())
 			{
				//signal_item_selected_in_treeView(item);
				emit signal_selectionChanged(item->get_handle());
				_cur_node_selected = item->get_handle();
				_selection_model = model;
				break;
			}
	 	}	
	}
}



void TreeViewForm::slot_find_node_in_tree()
{
	bool hasCurrent = _selection_model.isValid();
	if (hasCurrent)
		ui.treeView->scrollTo(_selection_model);
}

void TreeViewForm::hide_tool_bar()
{
	ui.toolBar->hide();
}

TreeViewForm* PythonWrapper_TreeViewForm::new_TreeViewForm( QWidget* parent /*= 0*/ )
{
	return new TreeViewForm(parent);
}
void PythonWrapper_TreeViewForm::delete_TreeViewForm( TreeViewForm* obj )
{
	delete obj;
}
void PythonWrapper_TreeViewForm::full_update_tree_form( TreeViewForm* obj )
{
	obj->full_update_tree_from_engine();
}
int PythonWrapper_TreeViewForm::get_cur_node_selected( TreeViewForm* obj )
{
	return obj->get_cur_node_selected();
}