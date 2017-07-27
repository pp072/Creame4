#ifndef DEBUG_WIDGET_H
#define DEBUG_WIDGET_H

#include <QWidget>
#include "GeneratedFiles/ui_debug_widget.h"

class debug_widget : public QWidget
{
	Q_OBJECT

public:
	debug_widget(QWidget *parent = 0);
	~debug_widget();
	Ui::debug_widget ui;
private:
	
};

#endif // DEBUG_WIDGET_H
