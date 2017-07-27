#ifndef GRIDVIEW_H
#define GRIDVIEW_H

#include "baseinstrument.h"

class GridView : public BaseInstrument
{
	Q_OBJECT

public:
	GridView(QObject *parent);
	~GridView();
	void render(EngineGrView * egv);
	void drawGrid();
private:
	
};

#endif // GRIDVIEW_H
