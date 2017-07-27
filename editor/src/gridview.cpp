#include "gridview.h"
#include "rcwindow.h"
#define stop __asm nop
GridView::GridView(QObject *parent)
	: BaseInstrument(parent, GuiInstrumentsType::Grid )
{

}

GridView::~GridView()
{

}

void GridView::render(EngineGrView * egv)
{
	if(egv != _egv)
		_egv = egv;
	if(_egv->getRcwindow()->ui.actionGridView->isChecked())
		drawGrid();
}

void GridView::drawGrid()
{
	float tx,ty,tz,rx,ry,rz,sx,sy,sz;
	h3dGetNodeTransform(_egv->getCam(), &tx, &ty, &tz,&rx, &ry, &rz, &sx, &sy, &sz);
	glColor3f(0.3f, 0.3f, 0.3f);
	glLineWidth(0.5);
	//glEnable( GL_MULTISAMPLE );
	

	glBegin(GL_LINES);
	for (int i=tz-50;i<tz+50;i+=1)
	{
		glVertex3f(tx+50, 0, i);
		glVertex3f(tx-50, 0, i);	
	}
 	for (int t=tx-50;t<tx+50;t+=1)
 	{
 		glVertex3f(t, 0, tz+50);
 		glVertex3f(t, 0, tz-50);	
 	}
	glEnd();

	glLineWidth(1);

	glColor3f(1.2f, 0.2f, 0.2f);
	
	glBegin(GL_LINES);
	glVertex3f(0+2, 0, 0);
	glVertex3f(0-2, 0, 0);
	glVertex3f(0, 0, 0+2);
	glVertex3f(0, 0, 0-2);
	glEnd();
	

	
}
