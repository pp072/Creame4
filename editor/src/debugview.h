#ifndef DEBUGVIEW_H
#define DEBUGVIEW_H

#include "baseinstrument.h"
#include "debug_widget.h"
#include <QPushButton>


class DebugView : public BaseInstrument
{
	Q_OBJECT

public:
	DebugView(QObject *parent);
	~DebugView();
	void render(EngineGrView * egv);
	void drawDebugInfo();
	EngineGrView *		_active_egv;
	void clear_light_buttons();

	void drawJointsInfo();
	void drawLightsInfo();

	debug_widget *_debug_widget;
	QVector <QPushButton *> _light_buttons;
private:
private slots:
	void selectNode();
	
};

#endif // DEBUGVIEW_H
