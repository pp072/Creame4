#ifndef CLASSIFICATIONOFAGENODE_H
#define CLASSIFICATIONOFAGENODE_H

#include <QObject>
#include <vector>
#include <QtGui>
class treeViewFormNode : public QObject
{
	// -- typedef
	typedef QSharedPointer<treeViewFormNode> clOfAge_ptr;
	typedef QList<clOfAge_ptr> list_clOfAge;

	Q_OBJECT
		Q_CLASSINFO("classificationOfAgeNode", tr("Basic:"));
	Q_PROPERTY(QString Name READ get_name WRITE set_name DESIGNABLE true USER true)
public:
	// -- enum	неизвестность, класс, группа, подгруппа, тип, вид, разновидность
	enum enum_gr{ unknown, root, klass, group, subgroup, type, view, form};
public:
	// -- properties
	QString									_name;
	QString									_handle;
	int										_type;
	enum_gr									_grouping;
	QSharedPointer<treeViewFormNode>		_parent;
	list_clOfAge							_childs;
public:
	// -- constructor, virtual destructor
	treeViewFormNode(QObject *parent = 0 , QString name = "", unsigned int handle = 0) :
	  QObject(parent), _grouping(root), _name(name)
	  {
		  _handle = QString::number(handle);
	  };
	virtual ~treeViewFormNode(){;};
	// -- methods
	int get_type() const { return _type; }
	void set_type(int val) { _type = val; }
	QString get_name() const;
	bool set_name(QString val);
	unsigned int get_handle() const { return _handle.toUInt(); }
	void set_handle(unsigned int val) { _handle = QString::number(val); }
	void set_handle(QString val) { _handle = val; }
	bool insert_children(int position, int count, int columns);
	int child_number();
	bool remove_children( int position, int count );
};

#endif // CLASSIFICATIONOFAGENODE_H