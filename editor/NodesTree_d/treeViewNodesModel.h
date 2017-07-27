#ifndef CLASSIFICATIONOFAGEMODEL_H
#define CLASSIFICATIONOFAGEMODEL_H
#include <QtGui>
#include <QAbstractItemModel>
#include "treeViewNodesNode.h"
class treeViewFormModel : public QAbstractItemModel
{
	Q_OBJECT

private:
	// -- properties
	QSharedPointer<treeViewFormNode> _root_node;
	
public:
	// -- constructor, virtual destructor
	treeViewFormModel(QObject *parent = 0);
	~treeViewFormModel();

	// -- methods
	QSharedPointer<treeViewFormNode> get_root_node() const { return _root_node; }
	void set_root_node(treeViewFormNode &node);
	void set_root_node();
	void reset_model();
	virtual bool insertRows(int position, int rows,
		const QModelIndex &parent = QModelIndex());
	virtual bool removeRows(int position, int rows,
		const QModelIndex &parent = QModelIndex());
	virtual Qt::ItemFlags flags(const QModelIndex &index)const;
	QModelIndex index(int row, int column,
		const QModelIndex &parent) const;
	virtual  QModelIndex parent(const QModelIndex &child) const;
	virtual int rowCount(const QModelIndex &parent) const;
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	QModelIndex create_index(int column, int row, void *data);
	virtual bool setData ( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole );
	virtual QVariant headerData(int section, Qt::Orientation orientation,
		int role) const;
	//	void mousePressEvent(QMouseEvent *event);
private:
	treeViewFormNode *node_from_index(const QModelIndex &index) const;
	
};

#endif // CLASSIFICATIONOFAGEMODEL_H
