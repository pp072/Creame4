#include "selectobjectbymouse.h"
#include "editor.h"
#include "treeViewNodesForm.h"
#include "Horde3DUtils.h"
#include "propertyeditor.h"

using namespace Horde3D;
#define stop __asm nop
SelectObjectByMouse::SelectObjectByMouse(QObject *parent)
	: BaseInstrument(parent, GuiInstrumentsType::SelectObjectByMouse ), m_gizmoSelection(AxeNONE)
{
	connect(ui.actionMouseSelectionMode,SIGNAL(triggered(bool)),this,SLOT(slot_selectionMode(bool)));
	connect(ui.actionMoveObject,SIGNAL(triggered(bool)),this,SLOT(slot_MoveMode(bool)));
	connect(ui.actionRotateObject,SIGNAL(triggered(bool)),this,SLOT(slot_RotateMode(bool)));
	connect(ui.actionScaleObject,SIGNAL(triggered(bool)),this,SLOT(slot_ScaleMode(bool)));

	connect(ui.actionLOC,SIGNAL(triggered(bool)),this,SLOT(slot_LOC_GLBMode(bool)));

	
	_cuurent_mode = None;
	_trans_mode = Local;
	ItemSelectedForRotate = 0;

	//QGraphicsScene *_graphic_scene =  _main_application->_rcWinMan->getPrimeWindow()->getEngineGrView()->getGraphicsScene();
	//EngineGrView * engrw = _main_application->_rcWinMan->getPrimeWindow()->getEngineGrView();
	//_line = _graphic_scene->addLine(0,0,0,0,QPen(QBrush(Qt::green),2));
	m_rotX = 0.0;
	m_rotY = 0.0;
	m_rotZ = 0.0;
}	

SelectObjectByMouse::~SelectObjectByMouse()
{

}
void SelectObjectByMouse::slot_selectionMode( bool onOff)
{
	if(onOff)
	{
		ui.actionMoveObject->setChecked(false);
		ui.actionRotateObject->setChecked(false);
		ui.actionScaleObject->setChecked(false);
		_cuurent_mode = InstrumentMode::Select;
	}
	else
	{
		_cuurent_mode = None;
	}
}
void SelectObjectByMouse::slot_MoveMode( bool onOff)
{
	if(onOff)
	{
		InstrumentsManager::get_instrument_manager_handle()->m_isMouseRelease = true;
		ui.actionMouseSelectionMode->setChecked(false);
		ui.actionRotateObject->setChecked(false);
		ui.actionScaleObject->setChecked(false);
		_cuurent_mode = InstrumentMode::Move;
	}
	else
	{
		_cuurent_mode = None;
	}
}
void SelectObjectByMouse::slot_RotateMode( bool onOff)
{
	if(onOff)
	{
		InstrumentsManager::get_instrument_manager_handle()->m_isMouseRelease = true;
		ui.actionMouseSelectionMode->setChecked(false);
		ui.actionMoveObject->setChecked(false);
		ui.actionScaleObject->setChecked(false);
		_cuurent_mode = InstrumentMode::Rotate;
	}
	else
	{
		_cuurent_mode = None;
	}
}
void SelectObjectByMouse::slot_ScaleMode( bool onOff)
{
	if(onOff)
	{
		InstrumentsManager::get_instrument_manager_handle()->m_isMouseRelease = true;
		ui.actionMouseSelectionMode->setChecked(false);
		ui.actionMoveObject->setChecked(false);
		ui.actionRotateObject->setChecked(false);
		_cuurent_mode = InstrumentMode::Scale;
	}
	else
	{
		_cuurent_mode = None;
	}
}
void SelectObjectByMouse::slot_LOC_GLBMode( bool isOn)
{
	if(isOn)
	{
		ui.actionLOC->setIcon(QIcon(":/instruments/icons/instruments/GLB.png"));
		_trans_mode = Global;
	}
	else
	{
		ui.actionLOC->setIcon(QIcon(":/instruments/icons/instruments/LOC.png"));
		_trans_mode = Local;
	}

}

void SelectObjectByMouse::slot_emit_MouseReleaseEventEngineGrView( QMouseEvent *event )
{
	_instruments_manager->m_isMouseRelease = true;
	QPoint mousePos = event->pos();
	//EngineGrView * egv = _main_application->_rcWinMan->getPrimeWindow()->getEngineGrView();
	QSize windsize = _egv->size();

	if(_cuurent_mode == Select && !(GetAsyncKeyState(83) < 0)) //!(GetAsyncKeyState(83) < 0) дл€ того чтобы не работало выделение при нажатой клавише s(вращение камеры)
	{
		H3DNode pickedNode=h3dutPickNode(_egv->getCam(),mousePos.x()/float(windsize.width()),1-mousePos.y()/float(windsize.height()));
		if (pickedNode>0x0)
		{
			H3DNode parentNode=h3dGetNodeParent(pickedNode);
			if(h3dGetNodeType(parentNode)==H3DNodeTypes::List::Model)
				pickedNode = parentNode;
			_main_application->_treeViewForm->selectByHandle(pickedNode);
			_main_application->ui.m_sceneTreeWidget->selectNode(pickedNode);
		}
	}
	unsigned int cur_selection = _main_application->get_cur_node_selected();
	if(cur_selection < 1) 
		return;
	else
		_main_application->getPropertyEditor()->slot_get_item_properties(cur_selection);

	if(_cuurent_mode != InstrumentMode::None && _cuurent_mode != InstrumentMode::Select)
	{
		h3dGetNodeTransform(currentItemSelected, &tx, &ty, &tz,&rx, &ry, &rz, &sx, &sy, &sz);
		emit transformChanged( tx, ty, tz,rx, ry, rz, sx, sy, sz);
	}
}
void SelectObjectByMouse::render_base()
{

 	unsigned int cur_selection = editor::get_main_window_handle()->get_cur_node_selected();
 	if(cur_selection < 1) 
		return;
	if(!h3dIsNodeValid( cur_selection )) 
		return;
 	if(cur_selection)
 		drawBoundingBox(cur_selection);
}

void SelectObjectByMouse::render()
{

	unsigned int cur_selection = editor::get_main_window_handle()->get_cur_node_selected();
	if(cur_selection < 1) 
		return;

	if(!h3dIsNodeValid( cur_selection )) 
		return;


	if(!_egv->isActiveWindow())
		return;

	glPushAtributesForGizmos();
	if (_cuurent_mode == InstrumentMode::Move)
	{
		drawMoveGizmo();
		moveObjectByMouse();
	
	}
	else if (_cuurent_mode==InstrumentMode::Rotate)
	{
		drawRotateGizmo();
		rotateObjectByMouse();
		
	}
	else if (_cuurent_mode==InstrumentMode::Scale)
	{
		drawScaleGizmo();
		scaleObjectByMouse();
	}
	glPopAttrib();
	
	
	
}

void SelectObjectByMouse::glPushAtributesForGizmos()
{
	glGetDoublev(GL_PROJECTION_MATRIX, projection);	// Retrieve The Projection Matrix		
	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetIntegerv(GL_VIEWPORT,viewport); 	// Get viewport settings for gluPickMatrix
	QPoint cursorPos = _egv->mapFromGlobal(QCursor::pos()); 

	winX = cursorPos.x();
	winY = viewport[3]-cursorPos.y();
	currentItemSelected=_main_application->get_cur_node_selected();
	

	gluUnProject( winX, winY, -1, modelview, projection, viewport, &posX0, &posY0, &posZ0);
	gluUnProject( winX, winY, 1, modelview, projection, viewport, &posX1, &posY1, &posZ1);

	p0=Vec3f(posX0, posY0, posZ0);
	p1=Vec3f(posX1, posY1, posZ1);


	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT | GL_HINT_BIT | GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);		
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST); 
	glClear(GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);
}
void SelectObjectByMouse::drawMoveGizmo(  )
{
	if(!InstrumentsManager::get_instrument_manager_handle()->m_isMouseRelease)
		drawShadowAxes();


	double projection[16];
	setupMatrices(projection);

	glMoveGizmo();
	GLuint selectBuf[512];
	checkGizmoSelection(projection, selectBuf);
	glMoveGizmo();	// Render Gizmo to retrieve the glName hits
	getGizmoNames(selectBuf);	

	restoreMatrices(projection);
}
void SelectObjectByMouse::drawShadowAxes()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	Matrix4f mat;
	if(_trans_mode == Local)
		mat.rotate( degToRad( rx ), degToRad( ry ), degToRad( rz ) );
	mat.translate( tx, ty, tz );
	float f[16];
	glMultMatrixf(mat.x);


	float camtx,camty,camtz,tmp;
	h3dGetNodeTransform(_egv->getCam(), &camtx, &camty, &camtz,&tmp, &tmp, &tmp, &tmp, &tmp, &tmp);
	glPushMatrix(); // Push matrix before applying the scaling for the gizmo
	float Distance = sqrt((tx-camtx)*(tx-camtx)+(ty-camty)*(ty-camty)+(tz-camtz)*(tz-camtz));
	glScalef(Distance/100,Distance/100,Distance/100);


	glColor3f( 0.5, 0.5, 0.5);	
	glDrawMoveAxe(Vec4f(0,1,0,90),0);
	glDrawMoveAxe(Vec4f(1,0,0,-90),0);
	glDrawMoveAxe(Vec4f(1,0,0,0),0);
	glPopMatrix();
	glPopMatrix();
}

void SelectObjectByMouse::drawRotateGizmo()
{
	double projection[16];
	setupMatrices(projection);

	glRotateGizmo();
	GLuint selectBuf[512];
	checkGizmoSelection(projection, selectBuf);
	glRotateGizmo();	// Render Gizmo to retrieve the glName hits
	getGizmoNames(selectBuf);

	restoreMatrices(projection);
}
void SelectObjectByMouse::drawScaleGizmo()
{
	double projection[16];
	setupMatrices(projection);

	glScaleGizmo();
	GLuint selectBuf[512];
	checkGizmoSelection(projection, selectBuf);
	glScaleGizmo();	// Render Gizmo to retrieve the glName hits
	getGizmoNames(selectBuf);

	restoreMatrices(projection);
}
void SelectObjectByMouse::setupMatrices( double * proj )
{
	float tx,ty,tz, rx,ry,rz, sx,sy,sz,tmp;
	h3dGetNodeTransform(_main_application->get_cur_node_selected(), &tx, &ty, &tz,&rx, &ry, &rz, &sx,&sy,&sz);

	//float tranmat[16];
	const float *absMatr;

	const float *tranmat;
	h3dGetNodeTransMats(_main_application->get_cur_node_selected(),&tranmat,&absMatr);

	
//	h3dGetNodeTransMats(_main_application->get_cur_node_selected(),0x0,&tmpMatr);
	Matrix4f trMatrix(absMatr);
	Vec3f trans, rot, scale;

	trMatrix.decompose( trans, rot, scale );

	//glTranslatef(tx,ty,tz);
	// Save current projection matrix
								// Where The 16 Doubles Of The Projection Matrix Are To Be Stored
	glGetDoublev(GL_PROJECTION_MATRIX, proj);	// Retrieve The Projection Matrix		

	GLdouble modelview[16];

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	// Set model view matrix of selected node
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	Matrix4f mat;
	if(_trans_mode == Local)
		mat.rotate( degToRad( rx ), degToRad( ry ), degToRad( rz ) );
	mat.translate( trans.x, trans.y, trans.z );
	float f[16];
	glMultMatrixf(mat.x);


	float camtx,camty,camtz;
	h3dGetNodeTransform(_egv->getCam(), &camtx, &camty, &camtz,&tmp, &tmp, &tmp, &tmp, &tmp, &tmp);
	glPushMatrix(); // Push matrix before applying the scaling for the gizmo
	float Distance = sqrt((tx-camtx)*(tx-camtx)+(ty-camty)*(ty-camty)+(tz-camtz)*(tz-camtz));
	glScalef(Distance/100,Distance/100,Distance/100);
}
void SelectObjectByMouse::checkGizmoSelection( double * projection, unsigned int * selectBuf )
{
	// Check for Gizmo Selection
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT,viewport); 	// Get viewport settings for gluPickMatrix
	QPoint cursorPos = _egv->mapFromGlobal(QCursor::pos()); // Retrieve mouse cursor position

	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	gluPickMatrix(cursorPos.x() ,viewport[3]-cursorPos.y(), 1, 1, viewport); // Use Pickmatrix to render only the scene under the mouse cursor
	glMultMatrixd(projection);
	glMatrixMode(GL_MODELVIEW);

			
	glSelectBuffer (512, selectBuf);
	glRenderMode (GL_SELECT);
	glInitNames();
	glPushName(0);
}
void SelectObjectByMouse::getGizmoNames( unsigned int * selectBuf)
{
	glFlush();
	glPopMatrix(); // Remove scale for Gizmo from stack	
	GLint hits = glRenderMode (GL_RENDER);

	GLuint names, *ptr = selectBuf;		
	float minAxisZ = float(INT_MAX);
	int axis = 0;

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT,viewport);
	QPoint cursorPos = _egv->mapFromGlobal(QCursor::pos()); // Retrieve mouse cursor position
	float winX = cursorPos.x();
	float winY = viewport[3]-cursorPos.y();

	for (int i = 0; i < hits; ++i) /*  for each hit  */
	{ 
		names = *ptr++;	// Get number of names for this hit			
		float z = float(*ptr)/0x7fffffff; // Get nearest Z Value
		ptr += 2; // Skip z2
		if (z < minAxisZ && names > 0) 
		{
			minAxisZ = z; 


			axis = *ptr; // Set selected axis


		}
		ptr += names;
	}


	if (InstrumentsManager::get_instrument_manager_handle()->m_isMouseRelease )
	{
		m_gizmoSelection = (Axis)axis;
		//ѕолучение координат выбранной точки на оси	
	}
}
void SelectObjectByMouse::restoreMatrices( double * projection )
{
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(projection);
	glMatrixMode(GL_MODELVIEW);
}

void SelectObjectByMouse::glMoveGizmo()
{
	InstrumentsManager *im=InstrumentsManager::get_instrument_manager_handle();
	glShadeModel(GL_SMOOTH);

	glPushMatrix ();
	
 		if(m_gizmoSelection==Axis::moveAxeX)
			glColor3f( 1, 1, 0.0);
		else
			glColor3f(1, 0, 0);
		glDrawMoveAxe(Vec4f(0,1,0,90),8);

 		if(m_gizmoSelection==Axis::moveAxeY)
			glColor3f( 1, 1, 0.0);
		else
			glColor3f( 0, 1, 0);
		glDrawMoveAxe(Vec4f(1,0,0,-90),16);

 		if(m_gizmoSelection==Axis::moveAxeZ)
			glColor3f( 1, 1, 0.0);
		else
			glColor3f( 0, 0, 1);
		glDrawMoveAxe(Vec4f(1,0,0,0),32);

	glPopMatrix ();
}
void SelectObjectByMouse::glDrawMoveAxe( Vec4f rotateAxe, int name )
{
	GLUquadricObj *quadObj = gluNewQuadric (); 
	glPushMatrix (); 
	glLoadName(name);
	glRotatef ((GLfloat) rotateAxe.w, (GLfloat) rotateAxe.x, (GLfloat) rotateAxe.y, (GLfloat) rotateAxe.z); 

	gluQuadricDrawStyle (quadObj, GLU_FILL); 
	gluQuadricNormals (quadObj, GLU_SMOOTH); 
	gluCylinder (quadObj, 0.100, 0.100, 3.75, 8, 2);
	gluDeleteQuadric(quadObj);
	glTranslatef ((GLfloat)0.0, (GLfloat)0.0, (GLfloat)3.5); 
	quadObj = gluNewQuadric (); 
	gluQuadricDrawStyle (quadObj, GLU_FILL); 
	gluQuadricNormals (quadObj, GLU_SMOOTH); 
	gluCylinder(quadObj, 0.5, 0.0, 1, 10, 6); 
	gluDeleteQuadric(quadObj);				
	glPopMatrix ();
}

void SelectObjectByMouse::glRotateGizmo()
{
	InstrumentsManager *im=InstrumentsManager::get_instrument_manager_handle();
	glShadeModel(GL_SMOOTH);
	glDisable(GL_CULL_FACE);
	// x-axis

	if(m_gizmoSelection==Axis::rotateAxeX)
		glColor3f( 1, 1, 0.0);
	else
		glColor3f(1, 0, 0);
	glDrawRotateAxe(Vec4f (0,1,0,90),64);

	if(m_gizmoSelection==Axis::rotateAxeY)
		glColor3f( 1, 1, 0.0);
	else
		glColor3f( 0, 1, 0);
	glDrawRotateAxe(Vec4f(1,0,0,-90),128);

	if(m_gizmoSelection==Axis::rotateAxeZ)
		glColor3f( 1, 1, 0.0);
	else
		glColor3f( 0, 0, 1);
	glDrawRotateAxe(Vec4f(1,0,0,0),256);

	glEnable(GL_CULL_FACE);
}
void SelectObjectByMouse::glDrawRotateAxe(  Horde3D::Vec4f rotateAxe, int name )
{
	GLUquadricObj *quadObj = gluNewQuadric (); 
	glPushMatrix (); 
	glLoadName(name);
	glRotatef ((GLfloat)rotateAxe.w, (GLfloat)rotateAxe.x, (GLfloat)rotateAxe.y, (GLfloat)rotateAxe.z); 

	gluQuadricDrawStyle (quadObj, GLU_FILL); 
	gluQuadricNormals (quadObj, GLU_SMOOTH); 
	gluDisk(quadObj, 4.5, 5, 24, 4);
	gluDeleteQuadric(quadObj);

	glTranslatef ((GLfloat)0.0, (GLfloat)0.0, (GLfloat)3.5); 
	quadObj = gluNewQuadric (); 
	gluQuadricDrawStyle (quadObj, GLU_FILL); 
	gluQuadricNormals (quadObj, GLU_SMOOTH); 
	gluCylinder(quadObj, 0.5, 0.0, 1, 10, 6); 
	gluDeleteQuadric(quadObj);	

	glPopMatrix ();
}

void SelectObjectByMouse::glScaleGizmo()
{
	unsigned int currentItemSelected=_main_application->get_cur_node_selected();
	InstrumentsManager *im=InstrumentsManager::get_instrument_manager_handle();
	glShadeModel(GL_SMOOTH);

	if(m_gizmoSelection==Axis::scaleAxeX)
		glColor3f( 1, 1, 0.0);
	else
		glColor3f(1, 0, 0);
	glDrawScaleAxe(Vec4f(0,1,0,90),512);

	if(m_gizmoSelection==Axis::scaleAxeY)
		glColor3f( 1, 1, 0.0);
	else
		glColor3f( 0, 1, 0);
	glDrawScaleAxe(Vec4f(1,0,0,-90),1024);
		
	if(m_gizmoSelection==Axis::scaleAxeZ)
		glColor3f( 1, 1, 0.0);
	else
		glColor3f( 0, 0, 1);
	glDrawScaleAxe(Vec4f(1,0,0,0),2048);	

}
void SelectObjectByMouse::glDrawScaleAxe( Horde3D::Vec4f rotateAxe, int name )
{
	GLUquadricObj *quadObj = gluNewQuadric ();
	glPushMatrix (); 
	glLoadName(name);

	glRotatef ((GLfloat)rotateAxe.w, (GLfloat)rotateAxe.x, (GLfloat)rotateAxe.y, (GLfloat)rotateAxe.z); 

	gluQuadricDrawStyle (quadObj, GLU_FILL); 
	gluQuadricNormals (quadObj, GLU_SMOOTH); 
	gluCylinder (quadObj, 0.1, 0.1, 3.75, 8, 2);
	gluDeleteQuadric(quadObj);			
	glPopMatrix ();
}

void SelectObjectByMouse::moveObjectByMouse()
{
	unsigned int currentItemSelected=_main_application->get_cur_node_selected();
	if (_cuurent_mode==InstrumentMode::Move && m_gizmoSelection!=Axis::AxeNONE)
	{
		if(!InstrumentsManager::get_instrument_manager_handle()->m_isMouseRelease && m_MouseButton == Qt::LeftButton)
		{
			Matrix4f mat;
			if(_trans_mode == Local)
				mat.rotate( degToRad( rx ), degToRad( ry ), degToRad( rz ) );
			Vec3f direction1;
			Vec3f direction2;

			switch(m_gizmoSelection)
			{
			case Axis::moveAxeX:
				direction1 = Vec3f(-1,0,0);
				direction2 = Vec3f(1,0,0);
				break;
			case Axis::moveAxeY:
				direction1 = Vec3f(0,-1,0);
				direction2 = Vec3f(0,1,0);
				break;
			case Axis::moveAxeZ:
				direction1 = Vec3f(0,0,-1);
				direction2 = Vec3f(0,0,1);
				break;
			}
			direction1 = mat * direction1;
			direction2 = mat * direction2;

			Vec3f int_point1 ;
			Vec3f int_point2 ;

			Vec3f int_point3 ;
			Vec3f int_point4 ;

			createIntersectionPlanes(direction1, direction2,&int_point1,&int_point2,&int_point3,&int_point4);


			//Ќахождение всех пересечений с плоскост€ми
			std::vector<std::vector<Vec3f>> v_win_pos;
			GLdouble winPosX, winPosY,winPosZ;

			std::vector<Vec3f> int_points_winPos;
			gluProject(int_point1.x,int_point1.y,int_point1.z,modelview, projection, viewport, &winPosX, &winPosY, &winPosZ);
			
			int_points_winPos.push_back(int_point1);int_points_winPos.push_back(Vec3f( winPosX, winPosY,winPosZ));
			v_win_pos.push_back(int_points_winPos);int_points_winPos.clear();

 			gluProject(int_point2.x,int_point2.y,int_point2.z,modelview, projection, viewport, &winPosX, &winPosY, &winPosZ);
			int_points_winPos.push_back(int_point2);int_points_winPos.push_back(Vec3f( winPosX, winPosY,winPosZ));
			v_win_pos.push_back(int_points_winPos);int_points_winPos.clear();

 			gluProject(int_point3.x,int_point3.y,int_point3.z,modelview, projection, viewport, &winPosX, &winPosY, &winPosZ);
			int_points_winPos.push_back(int_point3);int_points_winPos.push_back(Vec3f( winPosX, winPosY,winPosZ));
			v_win_pos.push_back(int_points_winPos);int_points_winPos.clear();

 			gluProject(int_point4.x,int_point4.y,int_point4.z,modelview, projection, viewport, &winPosX, &winPosY, &winPosZ);
			int_points_winPos.push_back(int_point4);int_points_winPos.push_back(Vec3f( winPosX, winPosY,winPosZ));
			v_win_pos.push_back(int_points_winPos);int_points_winPos.clear();
			
			Vec3f fin_point_dist;
			Vec3f fin_point;
			for (int i = 0; i < v_win_pos.size(); i++)
			{
				if(i==0)
				{
					fin_point_dist = v_win_pos[0][1];
					fin_point = v_win_pos[0][0];
					continue;
				}
				float D_old = sqrtf((fin_point_dist.x - winX)*(fin_point_dist.x - winX)+(fin_point_dist.y - winY)*(fin_point_dist.y - winY));
				float D_new = sqrtf((v_win_pos[i][1].x - winX)*(v_win_pos[i][1].x - winX)+(v_win_pos[i][1].y - winY)*(v_win_pos[i][1].y - winY));

				if(D_new<D_old)
				{
					fin_point_dist =  v_win_pos[i][1];
					fin_point = v_win_pos[i][0];
				}
			}

			//EngineGrView * engrw = _main_application->_rcWinMan->getPrimeWindow()->getEngineGrView();
			//_line->setLine(engrw->width()/2,engrw->height()/2,winX,viewport[3]-winY);

			//float angle = acos(fin_point.dot(Vec3f(winX-engrw->width()/2,viewport[3]-winY-engrw->height()/2,0).normalized()));
			//_main_application->_rcWinMan->getPrimeWindow()->getEngineGrView()->getDebug_widget()->ui.d_float_value_1->setValue(radToDeg(angle));
			//–азнца положени€ с начальной позицией
			if(m_pos.length() == 0.0)
			{
				m_pos.x = fin_point.x - tx;
				m_pos.y = fin_point.y - ty;
				m_pos.z = fin_point.z - tz;
			}
 			h3dSetNodeTransform(currentItemSelected,fin_point.x-m_pos.x ,fin_point.y-m_pos.y ,fin_point.z-m_pos.z ,rx, ry, rz, sx, sy, sz);	
			//emit moveObject(fin_point.x-m_pos.x ,fin_point.y-m_pos.y ,fin_point.z-m_pos.z);
		}
		else
		{
			h3dGetNodeTransform(currentItemSelected, &tx, &ty, &tz,&rx, &ry, &rz, &sx, &sy, &sz);
			m_pos=Vec3f(0,0,0);
		}
	}
	float tx,ty,tz,rx,ry,rz,sx,sy,sz;
	h3dGetNodeTransform(currentItemSelected, &tx, &ty, &tz,&rx, &ry, &rz, &sx, &sy, &sz);

}
void SelectObjectByMouse::createIntersectionPlanes(Vec3f direction1, Vec3f direction2,Horde3D::Vec3f *int_point1,Horde3D::Vec3f *int_point2,Horde3D::Vec3f *int_point3,Horde3D::Vec3f *int_point4 )
{
	H3DNode camera =  _egv->getCam();
	const float* cameraTrans = 0;
	h3dGetNodeTransMats(camera, 0, &cameraTrans);

	float matf[16];
	for (int i=0; i<16;i++)
		matf[i] = projection[i];
	Matrix4f m = Matrix4f(matf) * Matrix4f(cameraTrans).inverted();
	Plane cam_plane_far = Plane( -(m.c[0][3] - m.c[0][2]), -(m.c[1][3] - m.c[1][2]),
		-(m.c[2][3] - m.c[2][2]), -(m.c[3][3] - m.c[3][2]) );	// Far

	GLdouble posX2, posY2, posZ2;
	gluUnProject( winX, viewport[3], -1, modelview, projection, viewport, &posX0, &posY0, &posZ0);
	gluUnProject( winX, viewport[3], 1, modelview, projection, viewport, &posX1, &posY1, &posZ1);
	Vec3f p2=Vec3f(posX0, posY0, posZ0);
	Vec3f p3=Vec3f(posX1, posY1, posZ1);

	gluUnProject( viewport[2], winY, -1, modelview, projection, viewport, &posX0, &posY0, &posZ0);
	gluUnProject( viewport[2], winY, 1, modelview, projection, viewport, &posX1, &posY1, &posZ1);
	Vec3f p4=Vec3f(posX0, posY0, posZ0);
	Vec3f p5=Vec3f(posX1, posY1, posZ1);

	Vec3f intersectPoint1 = Vec3f(cam_plane_far.normal.x*2,cam_plane_far.normal.y*2,cam_plane_far.normal.z*2);
	Vec3f intersectPoint2 = Vec3f(cam_plane_far.normal.x*4,cam_plane_far.normal.y*4,cam_plane_far.normal.z*4);
	Vec3f intersectPoint3 = Vec3f(cam_plane_far.normal.x*8,cam_plane_far.normal.y*8,cam_plane_far.normal.z*8);
	Vec3f intersectPoint4 =  Vec3f(cam_plane_far.normal.x*8,cam_plane_far.normal.y*8,cam_plane_far.normal.z*8);

	int is_int1 = intersect(cam_plane_far,p0,p1,&intersectPoint1);
	int is_int2 = intersect(cam_plane_far,p0,p1,&intersectPoint2);
	int is_int3 = intersect(cam_plane_far,p2,p3,&intersectPoint3);
	int is_int4 = intersect(cam_plane_far,p4,p5,&intersectPoint4);


	Plane trann1(intersectPoint3,intersectPoint2,intersectPoint1);
	Plane trann2(intersectPoint1,intersectPoint2,intersectPoint3);

	Plane trann3(intersectPoint4,intersectPoint2,intersectPoint1);
	Plane trann4(intersectPoint1,intersectPoint2,intersectPoint4);


	const float farPlane = h3dGetNodeParamF(camera, H3DCamera::FarPlaneF, 0);
	float row_distance = farPlane*farPlane;

	*int_point1 = *int_point2 = *int_point3 = *int_point4 = intersectPoint2;
	

	int is_inter = intersect(trann1,Vec3f(tx,ty,tz),Vec3f(tx+direction1.x*row_distance,ty+direction1.y*row_distance,tz+direction1.z*row_distance),int_point1);
	if (!is_inter)
		is_inter = intersect(trann2,Vec3f(tx,ty,tz),Vec3f(tx+direction1.x*row_distance,ty+direction1.y*row_distance,tz+direction1.z*row_distance),int_point1);


	is_inter = intersect(trann1,Vec3f(tx,ty,tz),Vec3f(tx+direction2.x*row_distance,ty+direction2.y*row_distance,tz+direction2.z*row_distance),int_point2);
	if (!is_inter)
		is_inter = intersect(trann2,Vec3f(tx,ty,tz),Vec3f(tx+direction2.x*row_distance,ty+direction2.y*row_distance,tz+direction2.z*row_distance),int_point2);


	is_inter = intersect(trann3,Vec3f(tx,ty,tz),Vec3f(tx+direction1.x*row_distance,ty+direction1.y*row_distance,tz+direction1.z*row_distance),int_point3);
	if (!is_inter)
		is_inter = intersect(trann4,Vec3f(tx,ty,tz),Vec3f(tx+direction1.x*row_distance,ty+direction1.y*row_distance,tz+direction1.z*row_distance),int_point3);


	is_inter = intersect(trann3,Vec3f(tx,ty,tz),Vec3f(tx+direction2.x*row_distance,ty+direction2.y*row_distance,tz+direction2.z*row_distance),int_point4);
	if (!is_inter)
		is_inter = intersect(trann4,Vec3f(tx,ty,tz),Vec3f(tx+direction2.x*row_distance,ty+direction2.y*row_distance,tz+direction2.z*row_distance),int_point4);
}

void SelectObjectByMouse::rotateObjectByMouse()
{
	unsigned int currentItemSelected=_main_application->get_cur_node_selected();

	if (_cuurent_mode==InstrumentMode::Rotate && m_gizmoSelection!=Axis::AxeNONE && m_MouseButton == Qt::LeftButton)
	{
		const float *tmpMatr;
		h3dGetNodeTransMats(currentItemSelected,0x0,&tmpMatr);
		Matrix4f trMatrix(tmpMatr);

		float camrx,camry,camrz,tmp;
		h3dGetNodeTransform(_egv->getCam(), &tmp, &tmp, &tmp,&camrx, &camry, &camrz, &tmp, &tmp, &tmp);
		Vec3f intersectPoint = Vec3f(tx,ty,tz);
		

		H3DNode camera =  _egv->getCam();
		const float* cameraTrans = 0;
		h3dGetNodeTransMats(camera, 0, &cameraTrans);

		float matf[16];
		for (int i=0; i<16;i++)
			matf[i] = projection[i];
		Matrix4f m = Matrix4f(matf) * Matrix4f(cameraTrans).inverted();
		Plane cam_plane_far = Plane( -(m.c[0][3] - m.c[0][2]), -(m.c[1][3] - m.c[1][2]),
			-(m.c[2][3] - m.c[2][2]), -(m.c[3][3] - m.c[3][2]) );	// Far


		int isInt = intersect(cam_plane_far,p0,p1,&intersectPoint);

		float angle;
		Matrix4f RotMtr;
		float tmptr;

		if(!InstrumentsManager::get_instrument_manager_handle()->m_isMouseRelease)
		{
			Vec3f direction1;
			Vec3f direction2;

			if (m_gizmoSelection == Axis::rotateAxeX)
			{
				direction1 = Vec3f(-1,0,0);
				direction2 = Vec3f(1,0,0);
			}
			if (m_gizmoSelection == Axis::rotateAxeZ)
			{
				direction1 = Vec3f(0,0,-1);
				direction2 = Vec3f(0,0,1);
			}
			if (m_gizmoSelection == Axis::rotateAxeY)
			{
				direction1 = Vec3f(0,-1,0);
				direction2 = Vec3f(0,1,0);
			}

			Matrix4f mat;
			if(_trans_mode == Local)
				mat.rotate( degToRad( rx ), degToRad( ry ), degToRad( rz ) );
			direction1 = mat * direction1;
			direction2 = mat * direction2;
			Plane plane1;plane1.normal = direction1;
			Plane plane2;plane2.normal = direction2;
			Vec3f dir;
			Vec3f int_point1(tx,ty,tz);
			Vec3f int_point2(tx,ty,tz);
			if(intersect(plane1,p0,p1,&int_point1))
				dir = direction1;
			else if(intersect(plane2,p0,p1,&int_point2))
			{
				dir = direction2;
				stop
			}


			h3dGetNodeTransform(currentItemSelected, &tx, &ty, &tz,&rx, &ry, &rz, &sx, &sy, &sz);
 			angle = acos(Vec3f(1,0,0).dot(Vec3f(intersectPoint.x-tx,intersectPoint.y-ty,0).normalized()));
 
			if(intersectPoint.y > ty)
				angle = -angle;
 			RotMtr.rotate(dir,radToDeg(angle)-m_rotZ);
		
// 			Vec3f rot,tmp;
// 			
   			trMatrix=RotMtr*trMatrix;
// 			trMatrix.decompose(tmp,rot,tmp);
		//	emit rotateObject(dir.x,dir.y,dir.z,(radToDeg(angle)-m_rotZ));

			h3dSetNodeTransMat(currentItemSelected,trMatrix.x);
			h3dGetNodeTransform(currentItemSelected, &tmptr, &tmptr, &tmptr,&rx, &ry, &rz, &sx, &sy, &sz);
			h3dSetNodeTransform(currentItemSelected, tx, ty, tz,rx, ry, rz, sx,sy, sz);
			
			/*trMatrix.translate(tx, ty, tz);*/
			//emit rotateObject(rot.x,rot.y,rot.z);
  			m_rotZ=radToDeg(angle);
		}
		else
		{
			h3dGetNodeTransform(currentItemSelected, &tx, &ty, &tz,&rx, &ry, &rz, &sx, &sy, &sz);
			angle = acos(Vec3f(1,0,0).dot(Vec3f(intersectPoint.x-tx,intersectPoint.y-ty,0).normalized()));
			m_rotZ=radToDeg(angle);
			if(intersectPoint.y > ty)
				m_rotZ = -m_rotZ;
		}

		//		emit enableUndo();
		//		emit disableRedo();
	}
}
void SelectObjectByMouse::scaleObjectByMouse()
{
	unsigned int currentItemSelected=_main_application->_treeViewForm->get_cur_node_selected();

	if (_cuurent_mode==InstrumentMode::Scale && m_gizmoSelection!=Axis::AxeNONE && m_MouseButton == Qt::LeftButton)
	{

		if(!InstrumentsManager::get_instrument_manager_handle()->m_isMouseRelease)
		{
			Matrix4f mat;
			mat.rotate( degToRad( rx ), degToRad( ry ), degToRad( rz ) );
			Vec3f direction1;
			Vec3f direction2;

			switch(m_gizmoSelection)
			{
			case Axis::scaleAxeX:
				direction1 = Vec3f(-1,0,0);
				direction2 = Vec3f(1,0,0);
				break;
			case Axis::scaleAxeY:
				direction1 = Vec3f(0,-1,0);
				direction2 = Vec3f(0,1,0);
				break;
			case Axis::scaleAxeZ:
				direction1 = Vec3f(0,0,-1);
				direction2 = Vec3f(0,0,1);
				break;
			}
			direction1 = mat * direction1;
			direction2 = mat * direction2;

			Vec3f int_point1 ;
			Vec3f int_point2 ;

			Vec3f int_point3 ;
			Vec3f int_point4 ;

			createIntersectionPlanes(direction1, direction2,&int_point1,&int_point2,&int_point3,&int_point4);


			//Ќахождение всех пересечений с плоскост€ми
			std::vector<std::vector<Vec3f>> v_win_pos;
			GLdouble winPosX, winPosY,winPosZ;

			std::vector<Vec3f> int_points_winPos;
			gluProject(int_point1.x,int_point1.y,int_point1.z,modelview, projection, viewport, &winPosX, &winPosY, &winPosZ);

			int_points_winPos.push_back(int_point1);int_points_winPos.push_back(Vec3f( winPosX, winPosY,winPosZ));
			v_win_pos.push_back(int_points_winPos);int_points_winPos.clear();

			gluProject(int_point2.x,int_point2.y,int_point2.z,modelview, projection, viewport, &winPosX, &winPosY, &winPosZ);
			int_points_winPos.push_back(int_point2);int_points_winPos.push_back(Vec3f( winPosX, winPosY,winPosZ));
			v_win_pos.push_back(int_points_winPos);int_points_winPos.clear();

			gluProject(int_point3.x,int_point3.y,int_point3.z,modelview, projection, viewport, &winPosX, &winPosY, &winPosZ);
			int_points_winPos.push_back(int_point3);int_points_winPos.push_back(Vec3f( winPosX, winPosY,winPosZ));
			v_win_pos.push_back(int_points_winPos);int_points_winPos.clear();

			gluProject(int_point4.x,int_point4.y,int_point4.z,modelview, projection, viewport, &winPosX, &winPosY, &winPosZ);
			int_points_winPos.push_back(int_point4);int_points_winPos.push_back(Vec3f( winPosX, winPosY,winPosZ));
			v_win_pos.push_back(int_points_winPos);int_points_winPos.clear();

			Vec3f fin_point_dist;
			Vec3f fin_point;
			for (int i = 0; i < v_win_pos.size(); i++)
			{
				if(i==0)
				{
					fin_point_dist = v_win_pos[0][1];
					fin_point = v_win_pos[0][0];
					continue;
				}
				float D_old = sqrtf((fin_point_dist.x - winX)*(fin_point_dist.x - winX)+(fin_point_dist.y - winY)*(fin_point_dist.y - winY));
				float D_new = sqrtf((v_win_pos[i][1].x - winX)*(v_win_pos[i][1].x - winX)+(v_win_pos[i][1].y - winY)*(v_win_pos[i][1].y - winY));

				if(D_new<D_old)
				{
					fin_point_dist =  v_win_pos[i][1];
					fin_point = v_win_pos[i][0];
				}
			}

			if(m_pos.length() == 0.0)
			{
				m_pos.x = fin_point.x - tx;
				m_pos.y = fin_point.y - ty;
				m_pos.z = fin_point.z - tz;
			}

		
			switch(m_gizmoSelection)
			{
			case Axis::scaleAxeX:
				h3dSetNodeTransform(currentItemSelected, tx, ty, tz,rx, ry, rz, m_sclOffsetX-(tx-fin_point.x+m_pos.x),m_sclOffsetY, m_sclOffsetZ);
				break;
			case Axis::scaleAxeY:
				h3dSetNodeTransform(currentItemSelected, tx, ty, tz,rx, ry, rz, m_sclOffsetX,m_sclOffsetY-(ty-fin_point.y+m_pos.y), m_sclOffsetZ);
				break;
			case Axis::scaleAxeZ:
				h3dSetNodeTransform(currentItemSelected, tx, ty, tz,rx, ry, rz, m_sclOffsetX,m_sclOffsetY, m_sclOffsetZ-(tz-fin_point.z+m_pos.z));
				break;
			}
		}
		else
		{
			h3dGetNodeTransform(currentItemSelected, &tx, &ty, &tz,&rx, &ry, &rz, &sx, &sy, &sz);
			m_sclOffsetX = sx;
			m_sclOffsetY = sy;
			m_sclOffsetZ = sz;
			m_pos=Vec3f(0,0,0);
		}

	}
}
void SelectObjectByMouse::drawBoundingBox( unsigned int cur_selection )
{
	
	glDisable(GL_DEPTH_TEST);
	// Draw AABB of selected object
	float minX, minY, minZ, maxX, maxY, maxZ;
	
	h3dGetNodeAABB(cur_selection, &minX, &minY, &minZ, &maxX, &maxY, &maxZ);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
	glVertex3f(minX, maxY, minZ);
	glVertex3f(minX, maxY, maxZ);
	glVertex3f(maxX, maxY, maxZ);
	glVertex3f(maxX, maxY, minZ);
	glVertex3f(minX, maxY, minZ);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(minX, minY, minZ);
	glVertex3f(maxX, minY, minZ);
	glVertex3f(maxX, maxY, minZ);
	glVertex3f(minX, maxY, minZ);
	glVertex3f(minX, minY, minZ);
	glVertex3f(minX, minY, maxZ);
	glVertex3f(maxX, minY, maxZ);
	glVertex3f(maxX, maxY, maxZ);
	glVertex3f(minX, maxY, maxZ);
	glVertex3f(minX, minY, maxZ);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(minX, maxY, minZ);
	glVertex3f(minX, maxY, maxZ);
	glVertex3f(maxX, minY, minZ);
	glVertex3f(maxX, minY, maxZ);
	glVertex3f(maxX, maxY, minZ);
	glVertex3f(maxX, maxY, maxZ);
	glEnd();

	glEnable(GL_DEPTH_TEST);
}
