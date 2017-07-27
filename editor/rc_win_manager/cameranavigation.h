#ifndef CAMERANAVIGATION_H
#define CAMERANAVIGATION_H

#include <QObject>
#include <QCursor>
#include <QMouseEvent>
#include "adv_camera.h"
#include "treeViewNodesForm.h"
#include <QLabel>
/*#include "baseinstrument.h"*/
class EngineGrView;
class CameraNavigation : public QObject
{
	

enum CameraMode
{
	See,
	Walk,
	Fly
};
enum Projection
{
	X,
	Y,
	Z
};
	Q_OBJECT
public:
	CameraNavigation(EngineGrView * parent);
	~CameraNavigation();
	Camera* getAdvCamera();
	void unproject();
	void init();

	double projection[16];							
	double modelview[16];
private :
	EngineGrView*	_egv;
	TreeViewForm*	_treeViewForm;
	Camera			_adv_camera;
	QCursor			m_CursorMove;
	QCursor			m_CursorRotate;
	QCursor			m_CursorCantRotate;
	QPoint			m_mousePosCounter;
	
	float			m_zoomSpeed;
	int				m_curentCameraMode;
	Vec3f			_old_intersectPoint;
	bool isRightMouseClicked;

	void set_camera_name();
protected:
	virtual void mouseMoveEvent( QMouseEvent *event );
private slots:
	void slot_cameraZoomSpeedChanged( double speed );
	void slot_switchCameraModeToSee(bool);
	void slot_switchCameraModeToWalk( bool );
	void slot_switchCameraModeToFly( bool );
	void slot_CameraFindTarget(bool);
	void slot_setProjectionX();
	void slot_setProjectionY();
	void slot_setProjectionZ();
	void slot_setWireframeView();
	void slot_setDebugView();
	void slot_selectCamera();
	void slot_set_cam_from_tree(unsigned int);
	void slot_emit_MouseMoveEventEngineGrView( QMouseEvent *event );
	void slot_emit_MouseReleaseEventEngineGrView( QMouseEvent *event );
	void slot_emit_MousePressEventEngineGrView( QMouseEvent *event );
	void slot_emit_MouseWheelEventEngineGrView(QWheelEvent *event);
};

#endif // CAMERANAVIGATION_H
