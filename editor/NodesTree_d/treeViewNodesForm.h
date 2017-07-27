#ifndef CLASSIFICATIONOFAGEFORM_H
#define CLASSIFICATIONOFAGEFORM_H

#include <QMainWindow>
#include "treeViewNodesModel.h"
#include "global.h"
#include "GeneratedFiles/ui_treeViewForm.h"
#include <QComboBox>
#include "SceneFile.h"

class NODESTREE_D_EXPORT TreeViewForm : public QMainWindow
{
	Q_OBJECT
public:
enum type
{
	Undefined = 0,
	Group,
	Model,
	Mesh,
	Joint,
	Light,
	Camera,
	Emitter
};
public:
	TreeViewForm(QWidget *parent = 0);
	~TreeViewForm();
	void setup_model();
	void hide_tool_bar();
/*c++ & python classes*/
	void full_update_tree_from_engine();
	bool full_update_tree_from_file(SceneFile* sceneFile);;
	int get_cur_node_selected(){ return _cur_node_selected; }
/*---------------------*/
	void update_nodes_from_engine(int RootNode, treeViewFormNode *parent_item );
	QSharedPointer<treeViewFormModel> getModel() const { return _model; }
	void selectByHandle(unsigned int Handle);
private:
	Ui::treeViewForm ui;
	QSharedPointer<treeViewFormModel>	_model;
	volatile unsigned int				_cur_node_selected;
	type								_filter;
	QComboBox *							_combo_box;
	QModelIndex							_selection_model;
	bool loadSceneGraph( QString sceneGraph );
public slots:
	void apply_filter(int);
private slots:
	void insert_child_row();
	void insert_row();
	void delete_row();
	void update_actions();
	void slot_full_update_tree();
	
	void slot_find_node_in_tree();
signals:
	void signal_selectionChanged(unsigned int node_selected);
	

	//	void signal_item_selected_in_treeView(treeViewFormNode *item);
};
/*python wrapper*/
class NODESTREE_D_EXPORT PythonWrapper_TreeViewForm : public QObject 
{
	Q_OBJECT
public slots:
	TreeViewForm*	new_TreeViewForm(QWidget*  parent = 0);
	void			delete_TreeViewForm(TreeViewForm* obj);
	void			full_update_tree_form(TreeViewForm* obj);
	int				get_cur_node_selected(TreeViewForm* obj);
};

#endif // CLASSIFICATIONOFAGEFORM_H
