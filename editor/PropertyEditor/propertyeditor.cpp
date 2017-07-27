#include "propertyeditor.h"
#include "Horde3D.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"
#include "qtgroupboxpropertybrowser.h"
#include "qtbuttonpropertybrowser.h"
#include <QtGui/QScrollArea>
#define stop __asm nop
PropertyEditor::PropertyEditor(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	_property_widget = 0x0;
	ui.setupUi(this);
}

PropertyEditor::~PropertyEditor()
{

}

void PropertyEditor::slot_get_item_properties( unsigned int scene_node )
{
// 	H3DNodeTypes::List scene_node_type = (H3DNodeTypes::List)(h3dGetNodeType(scene_node));
 	_cur_node = scene_node;
 	delete_prop_widget();
 	_property_widget = new QWidget(this);
 
	_stand_prop_widget = new stand_properties();
 	_scroll_area = new QScrollArea(_property_widget);
 	_scroll_area->setWidgetResizable(true);
 	_scroll_area->setWidget(_stand_prop_widget);
	//ui.PropLayout->addWidget(_scroll_area);
	this->setCentralWidget(_scroll_area);
	set_standard_properties();

 	connect(_stand_prop_widget, SIGNAL(signal_values_changed()),
 		this, SLOT(slot_stand_values_changed()));
 
 	_property_widget->show();
	stop
}
void PropertyEditor::set_standard_properties()
{
	float tx, ty, tz;
	float rx, ry, rz;
	float sx, sy, sz;
	h3dGetNodeTransform(_cur_node, &tx, &ty, &tz, &rx, &ry, &rz, &sx, &sy, &sz);
	QString node_name = h3dGetNodeParamStr( _cur_node, H3DNodeParams::NameStr );

 	_stand_prop_widget->set_name(node_name);
	_stand_prop_widget->set_transformations(tx, ty, tz, rx, ry, rz, sx, sy, sz);

}



void PropertyEditor::slot_stand_values_changed( )
{
	float tx, ty, tz;
	float rx, ry, rz;
	float sx, sy, sz;
	_stand_prop_widget->get_transformations(&tx, &ty, &tz, &rx, &ry, &rz, &sx, &sy, &sz);	
 		h3dSetNodeTransform(_cur_node,  
			tx, ty, tz,
			rx, ry, rz,
			sx, sy, sz);
}

void PropertyEditor::delete_prop_widget()
{
 	if(_property_widget)
 	{
// 		//delete _tree_prop_editor;
// 		//delete _variant_manager;
// 		//
 		delete _property_widget;
 		delete _scroll_area;
// 		_propertyToId.clear();_idToProperty.clear();
 		_property_widget = 0x0;
 	}
}
