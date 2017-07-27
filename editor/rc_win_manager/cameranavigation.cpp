#include "cameranavigation.h"
#include "enginegrview.h"
#include <QPixmap>
#include "GlutStuff.h"
#include "rcwindow.h"
#include "rcwinman.h"

#define stop __asm nop
CameraNavigation::CameraNavigation(EngineGrView *parent )
: QObject((QObject*)parent),_egv(parent), m_mousePosCounter(QPoint(0,0)), m_zoomSpeed(0), _treeViewForm(0x0)
{
	m_CursorMove=QCursor(QPixmap(":/instruments/icons/instruments/camera_nav/move2.png"));
	m_CursorRotate=QCursor(QPixmap(":/instruments/icons/instruments/camera_nav/rotate_can_rotate.png"));
	m_CursorCantRotate=QCursor(QPixmap(":/instruments/icons/instruments/camera_nav/rotate_canNOT.png"));


	connect(_egv,SIGNAL(signal_emit_MouseMoveEvent(QMouseEvent *)), this,SLOT(slot_emit_MouseMoveEventEngineGrView(QMouseEvent *)));
	connect(_egv,SIGNAL(signal_emit_MouseReleaseEvent( QMouseEvent * )), this,SLOT(slot_emit_MouseReleaseEventEngineGrView( QMouseEvent * )));
	connect(_egv,SIGNAL(signal_emit_MouseWheelEvent(QWheelEvent *)), this,SLOT(slot_emit_MouseWheelEventEngineGrView(QWheelEvent *)));
	connect(_egv,SIGNAL(signal_emit_MousePressEvent(QMouseEvent *)), this,SLOT(slot_emit_MousePressEventEngineGrView(QMouseEvent *)));

//	connect(ui.spinBoxCameraZoomSpeed, SIGNAL(valueChanged(double)), this, SLOT(slot_cameraZoomSpeedChanged(double)));
//	slot_cameraAccelerationChanged(ui.spinBoxCameraAccel->value());
//	slot_cameraZoomSpeedChanged( ui.spinBoxCameraZoomSpeed->value());
//	ui.actionMouseSelectionMode,SIGNAL(triggered(bool)),this,SLOT(slot_selectionMode(bool)));

	connect(_egv->getRcwindow()->ui.actionSeeCamera, SIGNAL(triggered(bool)), this, SLOT(slot_switchCameraModeToSee(bool)));
	connect(_egv->getRcwindow()->ui.actionWalkCamera, SIGNAL(triggered(bool)), this, SLOT(slot_switchCameraModeToWalk(bool)));
	connect(_egv->getRcwindow()->ui.actionFlyCamera, SIGNAL(triggered(bool)), this, SLOT(slot_switchCameraModeToFly(bool)));
	connect(_egv->getRcwindow()->ui.actionFindTarget, SIGNAL(triggered(bool)), this, SLOT(slot_CameraFindTarget(bool)));

	connect(_egv->getRcwindow()->ui.actionSetXProjection, SIGNAL(triggered(bool)), this, SLOT(slot_setProjectionX()));
	connect(_egv->getRcwindow()->ui.actionSetYProjection, SIGNAL(triggered(bool)), this, SLOT(slot_setProjectionY()));
	connect(_egv->getRcwindow()->ui.actionSetZProjection, SIGNAL(triggered(bool)), this, SLOT(slot_setProjectionZ()));

	connect(_egv->getRcwindow()->ui.actionWireframeView, SIGNAL(triggered(bool)), this, SLOT(slot_setWireframeView()));
	connect(_egv->getRcwindow()->ui.actionDebugView, SIGNAL(triggered(bool)), this, SLOT(slot_setDebugView()));

	connect(_egv->getRcwindow()->ui.actionSelectCamera, SIGNAL(triggered(bool)), this, SLOT(slot_selectCamera()));
	
	
	m_zoomSpeed = 0.1;
	isRightMouseClicked = false;
	
	


}

void CameraNavigation::init()
{
	slot_switchCameraModeToWalk(true);
	_adv_camera.moveByMouseOrtho(Vec3f(0,0,-1));
	
	slot_switchCameraModeToSee(true);
	slot_setProjectionX();
	set_camera_name();

}

CameraNavigation::~CameraNavigation()
{

}

void CameraNavigation::mouseMoveEvent( QMouseEvent *event )
{
	
}

 void CameraNavigation::slot_emit_MouseMoveEventEngineGrView( QMouseEvent *event )
 {
		QPoint mousePos = event->pos();
		//GetAsyncKeyState(83) - S Keyboard
 		if( event->buttons() & Qt::LeftButton && GetAsyncKeyState(VK_LCONTROL) < 0 && m_curentCameraMode == CameraMode::Walk || 
			event->buttons() & Qt::LeftButton && GetAsyncKeyState(VK_LCONTROL) < 0 && m_curentCameraMode == CameraMode::Fly ||
			event->buttons() & Qt::LeftButton && GetAsyncKeyState(83) < 0 && m_curentCameraMode == CameraMode::See)
 		{
			qApp->setOverrideCursor(m_CursorRotate);

 			//Если мышка двигается первый раз
 			
 			if (m_mousePosCounter.x()==0 && m_mousePosCounter.y()==0)
 			{
 				m_mousePosCounter.setX(mousePos.x());
 				m_mousePosCounter.setY(mousePos.y());
 			}	
 			//Вычисляем смшение курсора
 			float diffX = mousePos.x() - m_mousePosCounter.x();
 			float diffY = mousePos.y() - m_mousePosCounter.y();
 			m_mousePosCounter = mousePos;
 
 
 			_egv->_rx += diffY ;
 			_egv->_ry -= diffX ;
		}else if( event->buttons() & Qt::RightButton && GetAsyncKeyState(VK_LCONTROL) < 0 && m_curentCameraMode == CameraMode::Walk || 
				  event->buttons() & Qt::LeftButton && GetAsyncKeyState(VK_LCONTROL) < 0 && m_curentCameraMode == CameraMode::Fly ||
				  event->buttons() & Qt::RightButton && GetAsyncKeyState(83) < 0 && m_curentCameraMode == CameraMode::See)
 		{
 			//_egv->setCursor(m_CursorMove);
			qApp->setOverrideCursor(m_CursorMove);
 			isRightMouseClicked = true;
 		}
 }

void CameraNavigation::slot_emit_MouseReleaseEventEngineGrView( QMouseEvent *event )
{
	_egv->unsetCursor();
	//qApp->restoreOverrideCursor();
	//qApp->setOverrideCursor(Qt::ArrowCursor);
	while( qApp->overrideCursor()) //be careful application may have been lock several times ...
		qApp->restoreOverrideCursor();
	m_mousePosCounter=QPoint(0,0);
	_old_intersectPoint = Vec3f(0,0,0);
	isRightMouseClicked = false;
}



void CameraNavigation::slot_emit_MouseWheelEventEngineGrView( QWheelEvent *event )
{
	_egv->_rz += event->delta()*m_zoomSpeed;
}

void CameraNavigation::slot_cameraZoomSpeedChanged( double speed )
{
	m_zoomSpeed = speed;
}

void CameraNavigation::slot_switchCameraModeToSee( bool on)
{

	if(on)
	{
		_egv->getRcwindow()->ui.actionWalkCamera->setChecked(false);
		_egv->getRcwindow()->ui.actionFlyCamera->setChecked(false);
		_egv->getRcwindow()->ui.actionSeeCamera->setChecked(true);
		_adv_camera.setBehavior(Camera::CAMERA_BEHAVIOR_ORBIT);

		

 		m_curentCameraMode=CameraMode::See;
	}else
		_egv->getRcwindow()->ui.actionSeeCamera->setChecked(true);
}

void CameraNavigation::slot_switchCameraModeToWalk( bool on)
{

  		_adv_camera.lookAt(Vec3f(0,0,1));
  		Matrix4f GlobMatr=_adv_camera.convertToCurrent();
 		h3dSetNodeTransMat(_adv_camera.GetCamera(),&GlobMatr.x[0]);

  		_egv->_ry=0;
  		_egv->_rx=0;
  		_egv->_rz=0;
  
  
  		_adv_camera.CAMERA_DIRRECTION=Vec3f(0,0,0);
		_adv_camera.convertToCurrent();

		if (on)
		{
			_egv->getRcwindow()->ui.actionSeeCamera->setChecked(false);
			_egv->getRcwindow()->ui.actionFlyCamera->setChecked(false);
			_egv->getRcwindow()->ui.actionWalkCamera->setChecked(true);
			_adv_camera.setBehavior(Camera::CAMERA_BEHAVIOR_FIRST_PERSON);
			m_curentCameraMode=CameraMode::Walk;	
		}else
			_egv->getRcwindow()->ui.actionWalkCamera->setChecked(true);
	
}

void CameraNavigation::slot_switchCameraModeToFly( bool on)
{
	if(on)
	{
		_egv->getRcwindow()->ui.actionWalkCamera->setChecked(false);
		_egv->getRcwindow()->ui.actionSeeCamera->setChecked(false);
		_egv->getRcwindow()->ui.actionFlyCamera->setChecked(true);
		_adv_camera.setBehavior(Camera::CAMERA_BEHAVIOR_FLIGHT);



		m_curentCameraMode=CameraMode::Fly;
	}else
		_egv->getRcwindow()->ui.actionSeeCamera->setChecked(true);
}

void CameraNavigation::slot_CameraFindTarget( bool )
{
	H3DNode sel_item = _egv->getRcwindow()->getRc_win_man()->getSelected_item();
	if( sel_item != 0 )
	{
		float minX, minY, minZ, maxX, maxY, maxZ;
		h3dGetNodeAABB(sel_item, &minX, &minY, &minZ, &maxX, &maxY, &maxZ);
		Vec3f BoundingCenter(minX+(maxX-minX)/2,minY+(maxY-minY)/2,minZ+(maxZ-minZ)/2);

		_adv_camera.lookAt(Vec3f(BoundingCenter.x,BoundingCenter.y,BoundingCenter.z));

	}
}

void CameraNavigation::slot_setProjectionX()
{
	_adv_camera.setCameraProjection(Camera::ProjectionModePlane::X);
}
void CameraNavigation::slot_setProjectionY()
{
	_adv_camera.setCameraProjection(Camera::ProjectionModePlane::Y);
}
void CameraNavigation::slot_setProjectionZ()
{
	_adv_camera.setCameraProjection(Camera::ProjectionModePlane::Z);
}
void CameraNavigation::slot_setWireframeView()
{
	_egv->_wireframeMode = !_egv->_wireframeMode;
}

void CameraNavigation::slot_setDebugView()
{
	_egv->_debugViewMode = !_egv->_debugViewMode;
}

void CameraNavigation::slot_emit_MousePressEventEngineGrView( QMouseEvent *event )
{

}

Camera* CameraNavigation::getAdvCamera()
{
	return &_adv_camera;
}

void CameraNavigation::unproject()
{
	if(!isRightMouseClicked)
		return;
	glGetDoublev(GL_PROJECTION_MATRIX, projection);	// Retrieve The Projection Matrix		
	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );


	GLint viewport[4];				
	glGetIntegerv(GL_VIEWPORT,viewport); 	// Get viewport settings for gluPickMatrix

	QPoint cursorPos = _egv->mapFromGlobal(QCursor::pos()); 

	float winX = cursorPos.x();
	float winY = viewport[3]-cursorPos.y();

	GLdouble posX0, posY0, posZ0;
	GLdouble posX1, posY1, posZ1;
	float tx,ty,tz,rx,ry,rz,sx,sy,sz;
	unsigned int currentItemSelected;

	gluUnProject( winX, winY, -1, modelview, projection, viewport, &posX0, &posY0, &posZ0);
	gluUnProject( winX, winY, 1, modelview, projection, viewport, &posX1, &posY1, &posZ1);

	Vec3f p0=Vec3f(posX0, posY0, posZ0);
	Vec3f p1=Vec3f(posX1, posY1, posZ1);


	H3DNode camera =  _adv_camera.GetCamera();
	const float* cameraTrans = 0;
	h3dGetNodeTransMats(camera, 0, &cameraTrans);

	float matf[16];
	for (int i=0; i<16;i++)
		matf[i] = projection[i];
	Matrix4f m = Matrix4f(matf) * Matrix4f(cameraTrans).inverted();
	Plane cam_plane_far = Plane( -(m.c[0][3] - m.c[0][2]), -(m.c[1][3] - m.c[1][2]),
		-(m.c[2][3] - m.c[2][2]), -(m.c[3][3] - m.c[3][2]) );// Far



	Vec3f intersectPoint = Vec3f(cam_plane_far.normal.x*2,cam_plane_far.normal.y*2,cam_plane_far.normal.z*2);

	int is_int1 = intersect(cam_plane_far,p0,p1,&intersectPoint);
	if (_old_intersectPoint == Vec3f(0,0,0))
	{
		_old_intersectPoint = intersectPoint;
		_old_intersectPoint = Matrix4f(cameraTrans).transposed() * _old_intersectPoint ;
	}

	stop
		//Вычисляем смшение курсора

		intersectPoint = Matrix4f(cameraTrans).transposed() * intersectPoint ;
	float diffX = _old_intersectPoint.x - intersectPoint.x ;
	float diffY = _old_intersectPoint.y - intersectPoint.y;



	_adv_camera.CAMERA_DIRRECTION.y +=diffY;	
	_adv_camera.CAMERA_DIRRECTION.x +=diffX;
//	_egv->getDebug_widget()->ui.d_float_value_1->setValue(radToDeg(_adv_camera.CAMERA_DIRRECTION.x));
//	_egv->getDebug_widget()->ui.d_float_value_2->setValue(radToDeg(_adv_camera.CAMERA_DIRRECTION.y));
}

void CameraNavigation::slot_selectCamera()
{
	/*_egv->getRcwindow()->getRc_win_man()->*/
	if(!_treeViewForm)
		_treeViewForm = new TreeViewForm(_egv);
	
	_treeViewForm->full_update_tree_from_engine();
	_treeViewForm->hide_tool_bar();
	_treeViewForm->apply_filter(TreeViewForm::type::Camera);
	
	connect(_treeViewForm, SIGNAL(signal_selectionChanged(unsigned int)), this, SLOT(slot_set_cam_from_tree(unsigned int)));

	_treeViewForm->setWindowFlags( Qt::Tool |  Qt::WindowStaysOnTopHint);
	_treeViewForm->move(QCursor::pos());
	_treeViewForm->show();
}

void CameraNavigation::slot_set_cam_from_tree( unsigned int cam)
{
	_treeViewForm->hide();
	_egv->setCam(cam);
	set_camera_name();

	//hack for resizing
	_egv->resize(_egv->size().width(),_egv->size().height()-1);
	_egv->resize(_egv->size().width(),_egv->size().height()+1);
}

void CameraNavigation::set_camera_name()
{
	//_egv->getRcwindow()->statusBar()->showMessage();
	_egv->getRcwindow()->statusBar()->hide();
	_egv->getRcwindow()->set_title(QString("camera: ") + h3dGetNodeParamStr(_egv->getCam(),H3DNodeParams::NameStr));
	
}
