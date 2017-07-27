#include "enginegrview.h"
#include "Horde3DUtils.h"
#include "utMath.h"
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "rcwindow.h"
#include <iomanip>
#include "GameObject.h"
//#include "editor.h"
#define stop __asm nop
EngineGrView::EngineGrView(QWidget *parent , bool isPrimeWindow)
	: QGraphicsView(parent), _isPrimeWindow(isPrimeWindow), _is_render(true)
{
	_isSceneLoaded = false;
	setWindowTitle(tr("Engine Graphics View"));

	_qglWidget = new QGLWidget(QGLFormat(QGL::SampleBuffers));
	_qglWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	if(_isPrimeWindow)
		setViewport(_qglWidget);
	//_glWidget->show();

	std::string _application_path =  qPrintable(QCoreApplication::applicationDirPath()); 
	_contentDir = _application_path + "/Content";
	
	

	_graphicsScene = new QGraphicsScene(this);
	this->setScene(_graphicsScene);	

	//_x = 5; _y = 3; _z = 50; _rx = 7; _ry = 15; _velocity = 10.0f;
	_ry = 0;
	_rx = 0;
	_rz = 0;

	_curFPS = 30;
	_oldTime = 0;
	_oldFPSTime = 0;
	frames = 0;
	_statMode = 0;
	_freezeMode = 0; _debugViewMode = false; _wireframeMode = false;
	_ctrl = false;
	_animTime = 0; _weight = 1.0f;
	_cam = 0;

	if(_isPrimeWindow)
	{
		initRenderWindow();
		setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
		initEngine();
		
	}
	_fontMatRes = h3dAddResource( H3DResTypes::Material, "overlays/font.material.xml", 0 );
	_panelMatRes = h3dAddResource( H3DResTypes::Material, "overlays/panel.material.xml", 0 );

	h3dutLoadResourcesFromDisk( _contentDir.c_str() );

	//createDebugGuiWidget();	
	
	stop
	//_rcWindow = 0x0;
	stop
	
}

EngineGrView::~EngineGrView()
{
// 	h3dutDumpMessages();
// 	h3dRelease();
}

void EngineGrView::keyPressEvent( QKeyEvent *event )
{
	switch( event->key() )
	{
	case Qt::Key_W:
		// Move forward
		if(_ctrl)
			_forward=true;
		break;
	case Qt::Key_S:
		// Move backward
		if(_ctrl)
			_backward=true;
		break;
	case Qt::Key_A:
		// Strafe left
		if(_ctrl)
			_left=true;
		break;
	case Qt::Key_D:
		// Strafe right
		if(_ctrl)
			_right = true;
		break;
	case Qt::Key_Control:
		// Strafe right
		_ctrl = true;
		break;
	}
	viewport()->update();
	//QGraphicsView::keyPressEvent(event);
}
void EngineGrView::keyReleaseEvent( QKeyEvent* event )
{
	switch( event->key() )
	{
	case Qt::Key_W:
		// Move forward
		_forward=false;
		break;
	case Qt::Key_S:
		// Move backward
		_backward=false;
		break;
	case Qt::Key_A:
		// Strafe left
		_left=false;
		break;
	case Qt::Key_D:
		// Strafe right
		_right = false;;
		break;
	case Qt::Key_Control:
		// Strafe right
		_ctrl = false;
		break;
	}
}
void EngineGrView::mouseMoveEvent( QMouseEvent *event )
{
	emit signal_emit_MouseMoveEvent(event);
	QGraphicsView::mouseMoveEvent(event);

}
void EngineGrView::mousePressEvent( QMouseEvent *event )
{
	switch(event->button())
	{
	case Qt::LeftButton:
		
		break;
	case Qt::RightButton:
		
		break;
	case Qt::MidButton:
		
		break;
	}
	emit signal_emit_MousePressEvent(event);
	QGraphicsView::mousePressEvent(event);
}

void EngineGrView::mouseReleaseEvent( QMouseEvent *event )
{
	emit signal_emit_MouseReleaseEvent(event);
	QGraphicsView::mouseReleaseEvent(event);
}

void EngineGrView::wheelEvent( QWheelEvent * event )
{
	emit signal_emit_MouseWheelEvent(event);
}

void EngineGrView::initRenderWindow()
{
	_QtHDC = (HDC)_qglWidget->getDC();
	// Init OpenGL rendering context
	int pixelFormat;

	static PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof( PIXELFORMATDESCRIPTOR ),			// Size of this pixel format descriptor
		1,											// Version number
		PFD_DRAW_TO_WINDOW |						// Format must support window
		PFD_SUPPORT_OPENGL |						// Format must support OpenGL
		PFD_DOUBLEBUFFER,							// Must support double buffering
		PFD_TYPE_RGBA,								// Request a RGBA format
		32,											// Select our color depth
		0, 0, 0, 0, 0, 0,							// Color bits ignored
		8,											// 8Bit alpha buffer
		0,											// Shift bit ignored
		0,											// No accumulation buffer
		0, 0, 0, 0,									// Accumulation bits ignored
		32,											// 32Bit z-buffer (depth buffer)  
		8,											// 8Bit stencil buffer
		0,											// No auxiliary buffer
		PFD_MAIN_PLANE,								// Main drawing layer
		0,											// Reserved
		0, 0, 0										// Layer masks ignored
	};

	pixelFormat = ChoosePixelFormat( _QtHDC, &pfd );
	SetPixelFormat( _QtHDC, pixelFormat, &pfd );
	_QtHGLRC = wglCreateContext( _QtHDC );
	wglMakeCurrent( _QtHDC, _QtHGLRC );

	_CrHDC = wglGetCurrentDC();
	_CrHGLRC = wglGetCurrentContext();

	
	
	stop

}
void EngineGrView::drawBackground( QPainter *painter, const QRectF &rect )
{
	if (painter->paintEngine()->type() != QPaintEngine::OpenGL2)
	{
		qWarning("OpenGLScene: drawBackground needs a QGLWidget to be set as viewport on the graphics view");
		return;
	}

	makeCurContext_engine();

	h3dSetOption( H3DOptions::WireframeMode, _wireframeMode );
	h3dSetOption( H3DOptions::DebugViewMode, _debugViewMode );

	_animTime += 1.0f / _curFPS;

	// Do animation blending
// 	h3dSetModelAnimParams( _knight, 0, _animTime * 24.0f, _weight );
// 	h3dSetModelAnimParams( _knight, 1, _animTime * 24.0f, 1.0f - _weight );
//	h3dUpdateModel( _knight, H3DModelUpdateFlags::Animation | H3DModelUpdateFlags::Geometry );


//	h3dSetNodeTransform( _cam, _x, _y, _z, _rx ,_ry, 0, 1, 1, 1 );
// 	const float ww = (float)h3dGetNodeParamI( _cam, H3DCamera::ViewportWidthI ) /
// 		(float)h3dGetNodeParamI( _cam, H3DCamera::ViewportHeightI );
// 	const float ovLogo[] = { ww-0.4f, 0.8f, 0, 1,  ww-0.4f, 1, 0, 0,  ww, 1, 1, 0,  ww, 0.8f, 1, 1 };
// 	h3dShowOverlays( ovLogo, 4, 1.f, 1.f, 1.f, 1.f, _logoMatRes, 0 );

	double elapsedTime=0;
	QTime initTime(QTime::currentTime());
	double curTime = (initTime.msec()/1000.0
		+(double)initTime.second()
		+(double)initTime.minute()*60.0
		+(double)initTime.hour()*60.0*60.0);

	elapsedTime =	curTime - _oldTime;

	_oldTime = curTime;
	SetCameraPosition();
	updateCamera(elapsedTime);

	
	
	if(_is_render)
	{
		emit signal_emit_RenderBefore(this,_animTime);
		h3dRender( _cam );
		drawAxis();
		setupProjections();
		
	}
	emit signal_emit_RenderReadyToOverlays(this);
	//FPS
	++frames;
	if( frames >= 3 )
	{
		_curFPS = frames / float(curTime - _oldFPSTime);
		frames = 0;
		_oldFPSTime = curTime;
	}
	

	show_status();
	if(_is_render)
		h3dFinalizeFrame();
	makeCurContext_QT();
}
void EngineGrView::drawAxis()
{


	  	glMatrixMode( GL_PROJECTION );
	  	glLoadIdentity();
	  	glOrtho( 0, 1, 0, 1, -1, 1 );
	  	glMatrixMode( GL_MODELVIEW );
	  	glLoadIdentity();
	  
	  	glDisable( GL_DEPTH_TEST );
	  
	  	glPushMatrix();
	  
	  	float	tx, ty, tz;	
	  	float	rx, ry, rz;
	  	float	sx, sy, sz;
	  	h3dGetNodeTransform(_cam,&tx, &ty, &tz,&rx, &ry, &rz,&sx, &sy, &sz);
	  
	  	QSize windsize=this->size();
	  
	  
	  	glTranslatef(0.01,0.01*windsize.width()/(float)windsize.height(),0);
	  	glScalef(1.0,windsize.width()/(float)windsize.height(),1.0);
	  	glRotatef(rx,1,0,0);
	  	glRotatef(ry,0,1,0);
	  	glRotatef(rz,0,0,1);
	  
	  
	  	glLineWidth(1.5);
	  	glColor3f(1.0,0.0,0.0);
	  	glBegin(GL_LINES);
	  	//X
	  	glVertex3f( 0, 0, 0 );	
	  	glVertex3f( 0 + 0.01 , 0 + 0 , 0 + 0  );
	  	glVertex3f( 0, 0, 0 );
	  	glVertex3f( 0  , 0  , 0 );
	  
	  	//Z
	  	glColor3f(0.0,0.0,1.0);
	  	glVertex3f( 0, 0, 0 );	
	  	glVertex3f( 0 + 0 , 0 + 0 , 0 - 0.01  );
	  	glVertex3f( 0, 0, 0 );
	  	glVertex3f( 0  , 0  , 0 );
	  
	  	//Y
	  	glColor3f(0.0,1.0,0.0);
	  	glVertex3f( 0, 0, 0 );	
	  	glVertex3f( 0 + 0 , 0 + 0.01 , 0 + 0  );
	  	glVertex3f( 0, 0, 0 );
	  	glVertex3f( 0  , 0  , 0 );
	  
	  	glEnd();
	  
	  
	  	glPopMatrix();
	  	glLineWidth(1.0);
	  	glEnable( GL_DEPTH_TEST );
}
bool EngineGrView::initEngine()
{
	if( !h3dInit() )
	{	
		h3dutDumpMessages();
		return false;
	}

	// Set options
	h3dSetOption( H3DOptions::LoadTextures, 1	);
	h3dSetOption( H3DOptions::TexCompression, 0 );
	h3dSetOption( H3DOptions::FastAnimation, 0	);
	h3dSetOption( H3DOptions::MaxAnisotropy, 4	);
	h3dSetOption( H3DOptions::ShadowMapSize, 2048 );

	return true;
}

void EngineGrView::resizeEvent( QResizeEvent *event )
{
	if (scene())
	{
		qWarning("Resizing graphics view");
		scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
	}

	float width = _qglWidget->width();
	float height = _qglWidget->height();

	makeCurContext_QT();
	makeCurContext_engine();

	h3dSetNodeParamI( _cam, H3DCamera::ViewportXI, 0 );
	h3dSetNodeParamI( _cam, H3DCamera::ViewportYI, 0 );
	h3dSetNodeParamI( _cam, H3DCamera::ViewportWidthI, width );
	h3dSetNodeParamI( _cam, H3DCamera::ViewportHeightI, height );

	// Set virtual camera parameters
	h3dSetupCameraView( _cam, 45.0f, (float)width / height, 0.1f, 1000.0f );
	int  pipeRes = h3dGetNodeParamI(_cam,H3DCamera::PipeResI);

	h3dResizePipelineBuffers(pipeRes , width, height );
	makeCurContext_QT();
}

void EngineGrView::makeCurContext_engine()
{
	wglMakeCurrent(_CrHDC, _CrHGLRC);
}

void EngineGrView::makeCurContext_QT()
{
	_qglWidget->makeCurrent();
}

void EngineGrView::loadScene()
{
	_isSceneLoaded=true;
}

void EngineGrView::initWindow()
{
	if(!_isPrimeWindow)
		setViewport(_qglWidget);
}

void EngineGrView::setupProjections()
{
	float proj[16];
	const float *invAbs;
	h3dGetCameraProjMat(_cam,proj);
	h3dGetNodeTransMats(_cam,0x0,&invAbs);
	Horde3D::Matrix4f curMat(invAbs);

	glMatrixMode( GL_PROJECTION );
	glLoadMatrixf( proj);
	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf( curMat.inverted().x );
}

void EngineGrView::createDebugGuiWidget()
{

	QFile styleFile(":/EditorContents/EditorContents/normalStyle.css");
	

	styleFile.open(QIODevice::ReadOnly);
	QByteArray  style= styleFile.readAll();
	//qApp->
	

	//QPushButton *button = new QPushButton("button");
	QGraphicsProxyWidget* proxy = new QGraphicsProxyWidget(0, Qt::Widget);
	_debug_widget = new debug_widget();
	_debug_widget->setStyleSheet(style);
	_debug_widget->setAttribute( Qt::WA_TranslucentBackground, true );
	QTransform trans;
	//trans.rotate(20,Qt::YAxis);
//	proxy->setTransform(trans);
	proxy->setOpacity(0.9);
	proxy->setWidget(_debug_widget);
	
	//proxy->setCacheMode(QGraphicsItem::ItemCoordinateCache);
	_graphicsScene->addItem(proxy);
	
}

void EngineGrView::setCam( H3DNode val )
{
	_cam = val;
	Camera* cam = _camera_nav->getAdvCamera();
	cam->setNodeCamera(_cam);
	_camera_nav->init();
}
void EngineGrView::SetCameraPosition()
{
	Camera* adv_cam = _camera_nav->getAdvCamera();
	if (_forward)
	{
		adv_cam->CAMERA_DIRRECTION.z -=1.0;
	}
	if (_backward)
	{
		adv_cam->CAMERA_DIRRECTION.z +=1.0f;
	}
	if (_left)
	{
		adv_cam->CAMERA_DIRRECTION.x -=1.0;
	}
	if (_right)
	{
		adv_cam->CAMERA_DIRRECTION.x +=1.0;
	}
	_forward=false;
	_backward=false;
	_left=false;
	_right=false;
}

void EngineGrView::updateCamera( double elapsedTime )
{
	
// 	CreameDLL::setRotationAdvCamera(_ry,-_rx, 0);


	_camera_nav->unproject();

	Camera* adv_cam = _camera_nav->getAdvCamera();
	if(_rx)
		stop
	adv_cam->rotateSmoothly(-_ry,_rx, 0);
	
	Matrix4f GlobMatr=adv_cam->convertToCurrent();
	h3dSetNodeTransMat(adv_cam->GetCamera(),&GlobMatr.x[0]);
// 	
 	_ry=0;_rx=0;
// 	
 	if (adv_cam->CAMERA_DIRRECTION.x!=0||adv_cam->CAMERA_DIRRECTION.y!=0||adv_cam->CAMERA_DIRRECTION.z!=0)
 	{
		//CreameDLL::setPositionAdvCameraOrtho(CAMERA_DIRRECTION.x, CAMERA_DIRRECTION.y, CAMERA_DIRRECTION.z);
		adv_cam->moveByMouseOrtho(Vec3f(adv_cam->CAMERA_DIRRECTION.x,adv_cam->CAMERA_DIRRECTION.y,adv_cam->CAMERA_DIRRECTION.z));
		Matrix4f GlobMatr=adv_cam->convertToCurrent();
		h3dSetNodeTransMat(adv_cam->GetCamera(),&GlobMatr.x[0]);
 		adv_cam->CAMERA_DIRRECTION=Vec3f(0,0,0);
 	}
 	if (_rz!=0)
 	{
 		//CreameDLL::setZoomAdvCamera(_rz);
		adv_cam->zoom(_rz/10.0 * adv_cam->getOrbitOffsetDistance()/20.0, adv_cam->getOrbitMinZoom(), adv_cam->getOrbitMaxZoom());
		Matrix4f GlobMatr=adv_cam->convertToCurrent();
		h3dSetNodeTransMat(adv_cam->GetCamera(),&GlobMatr.x[0]);
 		_rz=0;
 	}
}

void EngineGrView::show_status()
{
	h3dClearOverlays();
	if(getRcwindow()->ui.actionShowEngineStatus->isChecked())
		h3dutShowFrameStats( _fontMatRes, _panelMatRes, 2 );
}

