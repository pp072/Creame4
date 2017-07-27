#ifndef ENGINEGRVIEW_H
#define ENGINEGRVIEW_H


#include <QGraphicsView>
#include <QtGui>
#include <QGLWidget>
#include <sstream>
#include <string>
#include "Horde3D.h"

#include "debug_widget.h"
#include "cameranavigation.h"
#include <sstream>
#include <string>

class RCWinMan;
class RCWindow;
class EngineGrView : public QGraphicsView
{
	Q_OBJECT
	friend RCWinMan;
	friend RCWindow;

public:
	EngineGrView(QWidget *parent = 0, bool isPrimeWindow = false);
	~EngineGrView();

	void initRenderWindow();
	bool initEngine();
	void drawBackground(QPainter *painter, const QRectF &rect);

	void show_status();

	void setupProjections();

	void initWindow();
	void loadScene();

	void makeCurContext_engine();
	void makeCurContext_QT();

	void resizeEvent(QResizeEvent *event);
	void keyPressEvent( QKeyEvent *event );
	void keyReleaseEvent( QKeyEvent* event );
	void mouseMoveEvent( QMouseEvent *event );
	void mousePressEvent( QMouseEvent *event );
	void mouseReleaseEvent(QMouseEvent *event);
	void wheelEvent( QWheelEvent * event );

	QGLWidget* getQglWidget() const { return _qglWidget; }
	void setCam(H3DNode val);
	H3DNode getCam() {return _cam;}

	void createDebugGuiWidget();
	QGraphicsScene * getGraphicsScene() const { return _graphicsScene; }
	debug_widget* getDebug_widget() const { return _debug_widget; }
	RCWindow * getRcwindow() const { return _rcwindow; }
	
	float              _x, _y, _z, _rx, _ry,_rz;  // Viewer position and orientation
	bool               _debugViewMode, _wireframeMode;
	void updateCamera( double elapsedTime );

	void stop_render() { _is_render = false; }
	void start_render() { _is_render = true; }
	
private:
	
	
	bool			_is_render;
	bool			_isPrimeWindow;
	HDC				_CrHDC;
	HGLRC			_CrHGLRC;

	HDC				_QtHDC ;
	HGLRC			_QtHGLRC ;

	QGraphicsScene *   _graphicsScene;
	debug_widget*	   _debug_widget;
	CameraNavigation   *_camera_nav;
	RCWindow			*_rcwindow;
	
	QGLWidget*		_qglWidget;
	
	bool			_isSceneLoaded;
	bool			_forward, _backward, _left, _right, _up, _down,_ctrl;
	
	int				_lastMouseXPos, _lastMouseYPos;
	
	//Из horde ----------------------------------------------------------------------
	//Навигация
	
	float              _velocity;  // Velocity for movement
	float              _curFPS;

	int                _statMode;
	int                _freezeMode;
	
	float              _animTime, _weight;

 	H3DRes             _fontMatRes, _panelMatRes;
 	
	H3DNode				_cam;

	std::string        _contentDir;
	void SetCameraPosition();
	void drawAxis();
	double			_oldTime;
	int frames;
	double			_oldFPSTime;
	std::stringstream  _text;
	//Из horde ----------------------------------------------------------------------
signals:
	void signal_emit_MouseMoveEvent(QMouseEvent *event);
	void signal_emit_MouseReleaseEvent( QMouseEvent *event );
	void signal_emit_MousePressEvent(QMouseEvent * event);
	void signal_emit_MouseWheelEvent(QWheelEvent* event);
	void signal_emit_RenderReadyToOverlays(EngineGrView *);
	void signal_emit_RenderBefore(EngineGrView *,float);
	



};

#endif // ENGINEGRVIEW_H
