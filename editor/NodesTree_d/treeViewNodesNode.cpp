#include "treeViewNodesNode.h"

QString treeViewFormNode::get_name() const
{
	return _name;
}

bool treeViewFormNode::set_name( QString val )
{
	_name = val;
	return val.isEmpty();
}

bool treeViewFormNode::insert_children( int position, int count, int columns )
{
	if (position < 0 || position > _childs.size())
		return false;

	for (int row = 0; row < count; ++row) {
		QVector<QVariant> data(columns);
		clOfAge_ptr item = QSharedPointer<treeViewFormNode>(new treeViewFormNode(this), &QObject::deleteLater);
		item->_parent = QWeakPointer<treeViewFormNode>(this);
		_childs.insert(position, item);
	}

	return true;
}

int treeViewFormNode::child_number()
{
	if (!_parent.isNull())
		return _parent.data()->_childs.indexOf(QWeakPointer<treeViewFormNode>(this));
	return 0;
}

bool treeViewFormNode::remove_children( int position, int count )
{
	if (position < 0 || position + count > _childs.size())
		return false;

	for (int row = 0; row < count; ++row)
		_childs.takeAt(position);

	return true;
}
