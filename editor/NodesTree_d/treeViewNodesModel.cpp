#include "treeViewNodesModel.h"
//#include "creameDLL.h"
#include "Horde3D.h"
treeViewFormModel::treeViewFormModel( QObject *parent /*= 0*/ )
	: QAbstractItemModel(parent)
{

}

treeViewFormModel::~treeViewFormModel()
{

}

void treeViewFormModel::set_root_node( treeViewFormNode& node )
{
	_root_node = QSharedPointer<treeViewFormNode>(&node, &QObject::deleteLater);
	reset();
}

void treeViewFormModel::set_root_node()
{
	_root_node = QSharedPointer<treeViewFormNode>(new treeViewFormNode(this), &QObject::deleteLater);
	_root_node->set_name("value");
	reset();
}

Qt::ItemFlags treeViewFormModel::flags( const QModelIndex &index ) const
{
	treeViewFormNode *node = node_from_index(index);
	

	if (!index.isValid())
		return Qt::ItemIsEnabled;

	if ( index.column() == 1 ) 
	{
		
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	}
		

	if ( index.column() == 0 )
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex treeViewFormModel::index( int row, int column, const QModelIndex &parent ) const
{

	if (parent.isValid() && parent.column() != 0)
		return QModelIndex();


	treeViewFormNode *parentItem = node_from_index(parent);
	QWeakPointer<treeViewFormNode> childItem = parentItem->_childs.value(row);
	if(childItem)
		return createIndex(row, column, childItem.data());
	else
		return QModelIndex();

	
	//classificationOfAgeNode *parentNode = node_from_index(parent);
	//return createIndex(row, column, parentNode->_childs[row].data());
}



QModelIndex treeViewFormModel::parent( const QModelIndex &index ) const
{
	if (!index.isValid())
		return QModelIndex();

	treeViewFormNode *childItem = node_from_index(index);
	QWeakPointer<treeViewFormNode> parentItem = childItem->_parent;
	if (parentItem == _root_node)
		return QModelIndex();
	//QWeakPointer<classificationOfAgeNode> grandparent_node = parentItem.data()->_parent;
	

	
	return createIndex(parentItem.data()->child_number(), 0, parentItem.data());
}

int treeViewFormModel::rowCount( const QModelIndex &parent ) const
{
	treeViewFormNode *parentNode = node_from_index(parent);
	if (!parentNode)
		return 0;
	return parentNode->_childs.count();
}

int treeViewFormModel::columnCount( const QModelIndex &parent ) const
{
	return 2;
}

QVariant treeViewFormModel::data( const QModelIndex &index, int role ) const
{
	if (!index.isValid()) return QVariant(); 

	treeViewFormNode *node = node_from_index(index);

	if ((role != Qt::DisplayRole) && (role != Qt::DecorationRole) && (role != Qt::BackgroundRole))
		return QVariant();

	if(role == Qt::DisplayRole)
	{
		switch (index.column()) {
			case 0:
				return node->_name;
			case 1:
				return node->_handle;
			default:
				return QVariant();
		}
	}
	else if (role == Qt::BackgroundRole)
	{
		switch (node->_type)
		{
		case H3DNodeTypes::Mesh:
			return QColor(1,255,255,35);
			break;
		case H3DNodeTypes::Joint:
			return QColor(255,102,0,35);
			break;
		case H3DNodeTypes::Light:
			return QColor(255,255,1,35);
			break;
		case H3DNodeTypes::Model:
			return QColor(1,1,255,35);
			break;
		case H3DNodeTypes::Camera:
			return QColor(255,255,255,35);
			break;
		case H3DNodeTypes::Emitter:
			return QColor(54,241,45,35);
			break;
		default:
			return QColor(255,255,1,35);
		}
	}
 	else if(role == Qt::DecorationRole )
 	{
 		if(index.column()==1)
		{
			switch (node->_type)
			{
			case H3DNodeTypes::Mesh:
				return QIcon(":/tree/icons/Cube.png");
				break;
			case H3DNodeTypes::Light:
				return QIcon(":/tree/icons/Light.png");
				break;
			case H3DNodeTypes::Model:
				return QIcon(":/tree/icons/3D_cube1.png");
				break;
			case H3DNodeTypes::Camera:
				return QIcon(":/tree/icons/Cam.png");
				break;
			case H3DNodeTypes::Emitter:
				return QIcon(":/tree/icons/Krugi_plotnosti_IC.png");
				break;
			case H3DNodeTypes::Joint:
				return QIcon(":/tree/icons/dog_bone.png");
				break;
			case H3DNodeTypes::Group:
				return QIcon(":/tree/icons/Group.png");
				break;
			default:
				return QIcon(":/tree/icons/Krugi_plotnosti_IC.png");
				break;
			}
		}
 	}
	return QVariant();
}

bool treeViewFormModel::setData( const QModelIndex & index, const QVariant & value, int role /*= Qt::EditRole */ )
{
	if (role != Qt::EditRole)
		return false;

// 	treeViewFormNode *item = node_from_index(index);
//  	bool result = item->set_name(value.toString());
// 	//bool result = false;
// 	if (result)
// 		emit dataChanged(index, index);

	return false;
}

QVariant treeViewFormModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		 switch (section) 
		{
			 case 0:
				 return tr("name:");
			 case 1:
				 return tr("node:");
		}
	}
	return QVariant();
}

treeViewFormNode * treeViewFormModel::node_from_index( const QModelIndex &index ) const
{
	if (index.isValid()) {
		treeViewFormNode *item =static_cast<treeViewFormNode *>(index.internalPointer());
		if (item) 
			return item;
	}
	return _root_node.data();
}

bool treeViewFormModel::insertRows( int position, int rows, const QModelIndex &parent /*= QModelIndex()*/ )
{
	//classificationOfAgeNode *parentItem = getItem(parent);
	treeViewFormNode *parentNode = node_from_index(parent);
	bool success;

	beginInsertRows(parent, position, position + rows - 1);
	success = parentNode->insert_children(position, rows, 1);
	endInsertRows();

	return success;
}

bool treeViewFormModel::removeRows( int position, int rows, const QModelIndex &parent /*= QModelIndex()*/ )
{
	treeViewFormNode *parentItem = node_from_index(parent);
	bool success = true;

	beginRemoveRows(parent, position, position + rows - 1);
	success = parentItem->remove_children(position, rows);
	endRemoveRows();

	return success;
}

void treeViewFormModel::reset_model()
{
	reset();
}

QModelIndex treeViewFormModel::create_index( int column, int row, void *data )
{
	return createIndex(column,row,data);
}
