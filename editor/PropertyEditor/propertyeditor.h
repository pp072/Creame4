#ifndef PROPERTYEDITOR_H
#define PROPERTYEDITOR_H

#include <QtGui/QMainWindow>
#include "GeneratedFiles/ui_propertyeditor.h"
#include "stand_properties.h"
#ifdef DLL_NONCLIENT_BUILD // в директивах препроцессора проекта с dll нужно прописать DLL_NONCLIENT_BUILD
#   define DLL_EXPORT __declspec(dllexport)
#else
#   define DLL_EXPORT __declspec(dllimport)
#endif

class QtProperty;
class DLL_EXPORT PropertyEditor : public QMainWindow
{
	Q_OBJECT

public:
	PropertyEditor(QWidget *parent = 0, Qt::WFlags flags = 0);
	~PropertyEditor();
private:
	Ui::PropertyEditorClass ui;

	QWidget	*						_property_widget;
	class QScrollArea *				_scroll_area;
	unsigned int					_cur_node;
	stand_properties *				_stand_prop_widget;

	void set_standard_properties();

public slots:
	void slot_get_item_properties(unsigned int item);
	void delete_prop_widget();
	void slot_stand_values_changed();


};

#endif // PROPERTYEDITOR_H
