#ifndef SELECTOBJECTBYMOUSE_H
#define SELECTOBJECTBYMOUSE_H

#include <QObject>
#include "baseinstrument.h"
#include "utMath.h"
#include "GlutStuff.h"
#include <QGraphicsItem>
enum InstrumentMode
{
	None,
	Select,
	Move,
	Rotate,
	Scale,
	AutoisternaPog
};
enum Axis
{
	AxeNONE =0,
	moveAxeX=8,
	moveAxeY=16,
	moveAxeZ=32,
	rotateAxeX=64,
	rotateAxeY=128,
	rotateAxeZ=256,
	scaleAxeX=512,
	scaleAxeY=1024,
	scaleAxeZ=2048,
	propScaleXZ=2049,
	propRotateY=2050,
	propTranslateXZ=2051

};
enum TransMode
{
	Local,
	Global
};
class SelectObjectByMouse : public BaseInstrument
{
	Q_OBJECT

public:
	SelectObjectByMouse(QObject *parent);
	~SelectObjectByMouse();
	InstrumentMode _cuurent_mode;
	Axis			m_gizmoSelection;
	TransMode		_trans_mode;
	void render();
	void drawBoundingBox( unsigned int cur_selection );
	void render_base();
private:
	void glPushAtributesForGizmos();
	void drawMoveGizmo();

	void drawShadowAxes();

	void checkGizmoSelection( double * projection, unsigned int * selectBuf );

	void setupMatrices(double * proj);

	void moveObjectByMouse();

	void createIntersectionPlanes(Horde3D::Vec3f direction1, Horde3D::Vec3f direction2,Horde3D::Vec3f *int_point1,Horde3D::Vec3f *int_point2,Horde3D::Vec3f *int_point3,Horde3D::Vec3f *int_point4 );

	void glMoveGizmo();

	
	void drawRotateGizmo();
	void rotateObjectByMouse();
	void glRotateGizmo();

	void glDrawMoveAxe( Horde3D::Vec4f rotateAxe, int name );
	void glDrawRotateAxe( Horde3D::Vec4f rotateAxe, int name );	
	void glDrawScaleAxe( Horde3D::Vec4f rotateAxe, int name );


	void drawScaleGizmo();

	void restoreMatrices( double * projection );

	void getGizmoNames( unsigned int * selectBuf);

	void glScaleGizmo();
	void scaleObjectByMouse();

	unsigned int ItemSelectedForRotate;
	

	bool is_can_update_plan;
	Horde3D::Vec3f m_pos;
	double m_sclX, m_sclY, m_sclZ;
	double m_rotX, m_rotY, m_rotZ;
	double m_sclOffsetX,m_sclOffsetY,m_sclOffsetZ;

	GLdouble projection[16];							// Where The 16 Doubles Of The Projection Matrix Are To Be Stored
	GLdouble modelview[16];
	GLint viewport[4];
	float winX ;
	float winY ;
	GLdouble posX0, posY0, posZ0;
	GLdouble posX1, posY1, posZ1;
	float tx,ty,tz,rx,ry,rz,sx,sy,sz;
	unsigned int currentItemSelected;
	Horde3D::Vec3f p0;
	Horde3D::Vec3f p1;


// 	QGraphicsScene *_scene;
// 	QGraphicsLineItem *_line;


protected:
	virtual void slot_emit_MouseReleaseEventEngineGrView(QMouseEvent *event);
	
private slots:
	void slot_selectionMode(bool);
	void slot_MoveMode(bool);
	void slot_RotateMode(bool);
	void slot_ScaleMode(bool);
	void slot_LOC_GLBMode(bool);
signals:
	void transformChanged(const float tx, const float ty, const float tz, const float rx, const float ry, const float rz, const float sx, const float sy, const float sz);
	
};

#endif // SELECTOBJECTBYMOUSE_H
