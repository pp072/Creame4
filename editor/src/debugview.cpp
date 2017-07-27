#include "debugview.h"
#include "rcwindow.h"

#include "utMath.h"
#include "GlutStuff.h"
#include "editor.h"
#define stop __asm nop
DebugView::DebugView(QObject *parent)
	: BaseInstrument(parent, GuiInstrumentsType::DebugView )
{
	_debug_widget = 0x0;
	_active_egv = 0x0;
}

DebugView::~DebugView()
{

}

void DebugView::render( EngineGrView * egv )
{
	if(egv != _egv)
		_egv = egv;
	
	drawDebugInfo();
	

}

void DebugView::drawDebugInfo()
{
	//glDisable(GL_DEPTH_TEST); 
	if(_egv->getRcwindow()->ui.actionDebugView2->isChecked())
	{
		glDisable(GL_DEPTH_TEST);
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		drawJointsInfo();
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST); 
	}

	if(_egv->hasFocus() && _egv->isActiveWindow() && _egv!= _active_egv)
	{
		clear_light_buttons();
		_active_egv = _egv;
	}

	if(_egv->getRcwindow()->ui.actionDebugView3->isChecked() )
		drawLightsInfo();
	else if( _egv->hasFocus())
		clear_light_buttons();

}

void DebugView::drawJointsInfo()
{
	int findsNodes = h3dFindNodes(1,"",H3DNodeTypes::Joint);
	for (int i = 0 ;i< findsNodes; i++)
	{
		float tx,ty,tz,rx,ry,rz,sx,sy,sz;

		int joint = h3dGetNodeFindResult(i);
		h3dGetNodeTransform(joint, &tx, &ty, &tz,&rx, &ry, &rz, &sx, &sy, &sz);

		
		const float *tmpMatr;
		h3dGetNodeTransMats(joint,0x0,&tmpMatr);
		Matrix4f trMatrix(tmpMatr);
		Vec3f trans, rot, scale;

		trMatrix.decompose( trans, rot, scale );

		int parent = h3dGetNodeParent(joint);

	//	glColor3f( 1.0, 1.0, 1.0);
		

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		Matrix4f mat;
		mat.translate( trans.x, trans.y, trans.z);
		float f[16];
		glMultMatrixf(mat.x);


		float camtx,camty,camtz,tmp;
		h3dGetNodeTransform(_egv->getCam(), &camtx, &camty, &camtz,&tmp, &tmp, &tmp, &tmp, &tmp, &tmp);
		glPushMatrix(); 
		float Distance = sqrt((tx-camtx)*(tx-camtx)+(ty-camty)*(ty-camty)+(tz-camtz)*(tz-camtz));
		//glScalef(Distance/100,Distance/100,Distance/100);


		glColor4f( 0.0, 1.0, 0.2, 0.5);	

		GLUquadricObj *quadObj = gluNewQuadric (); 
		glPushMatrix (); 


		gluQuadricDrawStyle (quadObj, GLU_FILL); 
		gluQuadricNormals (quadObj, GLU_SMOOTH); 
		gluSphere(quadObj, 0.05,  8, 8);
		gluDeleteQuadric(quadObj);
		glPopMatrix ();


		glPopMatrix();
		glPopMatrix();

		if(h3dGetNodeType(parent) == H3DNodeTypes::Joint)
		{
			int par_index = h3dGetNodeParamI(parent,H3DJoint::JointIndexI);
			int joint_index = h3dGetNodeParamI(joint,H3DJoint::JointIndexI);
			if(!par_index /*|| !joint_index*/)
				continue;
			glColor4f(1.0,1.0,1.0, 0.5);
			glBegin(GL_LINES);
			glVertex3f(trans.x, trans.y, trans.z);

			h3dGetNodeTransMats(parent,0x0,&tmpMatr);
			trMatrix = Matrix4f(tmpMatr);
			trMatrix.decompose( trans, rot, scale );

			glVertex3f(trans.x, trans.y, trans.z);	
			glEnd();
		}

	}
}

void DebugView::drawLightsInfo()
{
	GLdouble projection[16];							// Where The 16 Doubles Of The Projection Matrix Are To Be Stored
	GLdouble modelview[16];
	GLint viewport[4];

	glGetDoublev(GL_PROJECTION_MATRIX, projection);	// Retrieve The Projection Matrix		
	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetIntegerv(GL_VIEWPORT,viewport); 	// Get viewport settings for gluPickMatrix

	
		
	glDisable(GL_CULL_FACE);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	int findsNodes = h3dFindNodes(1,"",H3DNodeTypes::Light);

	if(_egv->hasFocus() && _egv->isActiveWindow())
	{
		for (int t = 0; t < _light_buttons.size(); ++t)
		{
			QString name = _light_buttons.at(t)->objectName();
			int IDNode =name.toInt();
			if(!h3dIsNodeValid(IDNode) || (h3dGetNodeType(IDNode) != H3DNodeTypes::Light))
			{
				clear_light_buttons();
				continue;
			}

			const float *tmpMatr;
			h3dGetNodeTransMats(IDNode,0x0,&tmpMatr);
			Matrix4f trMatrix(tmpMatr);
			Vec3f trans, rot, scale;

			trMatrix.decompose( trans, rot, scale );

			GLdouble winPosX, winPosY,winPosZ;
			std::vector<Vec3f> int_points_winPos;
			gluProject(trans.x,trans.y,trans.z,modelview, projection, viewport, &winPosX, &winPosY, &winPosZ);
			if(winPosZ<1.0)
				_light_buttons.at(t)->move(winPosX-10,_egv->height()-winPosY);
			else
				_light_buttons.at(t)->move(-100,_egv->height()-winPosY);

		}
	}
	for (int i = 0 ;i< findsNodes; i++)
	{

		bool isFind = false;
		for (int t = 0; t < _light_buttons.size(); ++t) {
			if (_light_buttons.at(t)->objectName()== QString::number(h3dGetNodeFindResult(i)))
				isFind = true;
		}

		if(!isFind && _egv->hasFocus() && _egv->isActiveWindow())
		{
			QPushButton * button = new QPushButton(QIcon(":/tree/icons/Light.png"),"",_egv);
			connect(button,SIGNAL(released()),this, SLOT(selectNode()));
			button->setObjectName(QString::number(h3dGetNodeFindResult(i)));
			_light_buttons.push_back(button);
			QGraphicsProxyWidget* proxy = new QGraphicsProxyWidget(0, Qt::Widget);
			proxy->setWidget(button);
			_egv->getGraphicsScene()->addItem(proxy);
			button->show();
		}

		


		if(_main_application->get_cur_node_selected()!=h3dGetNodeFindResult(i))
			continue;
		float radius = h3dGetNodeParamF(h3dGetNodeFindResult(i),H3DLight::RadiusF,0);
		float fov = h3dGetNodeParamF(h3dGetNodeFindResult(i),H3DLight::FovF,0);
		float r = radius * tanf( degToRad( fov / 2 ) );

		float tx,ty,tz,rx,ry,rz,sx,sy,sz;
		h3dGetNodeTransform(h3dGetNodeFindResult(i), &tx, &ty, &tz,&rx, &ry, &rz, &sx, &sy, &sz);
		//h3dGetNodeTransMats(h3dGetNodeFindResult(i),)

		const float *tmpMatr;
		h3dGetNodeTransMats(h3dGetNodeFindResult(i),0x0,&tmpMatr);
		Matrix4f trMatrix(tmpMatr);
		Vec3f trans, rot, scale;

		trMatrix.decompose( trans, rot, scale );

		


		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		Matrix4f mat;

		if(fov < 180.0)
			mat.translate( 0, 0, -radius);
		mat.rotate( rot.x, rot.y, rot.z);
		mat.translate( trans.x, trans.y, trans.z);
		
		float f[16];
		glMultMatrixf(mat.x);


		float camtx,camty,camtz,tmp;
		h3dGetNodeTransform(_egv->getCam(), &camtx, &camty, &camtz,&tmp, &tmp, &tmp, &tmp, &tmp, &tmp);
		glPushMatrix(); 
		float Distance = sqrt((tx-camtx)*(tx-camtx)+(ty-camty)*(ty-camty)+(tz-camtz)*(tz-camtz));
		//glScalef(Distance/100,Distance/100,Distance/100);

		GLUquadricObj *quadObjFill = gluNewQuadric (); 
		GLUquadricObj *quadObjLine = gluNewQuadric (); 
		glPushMatrix (); 


		gluQuadricDrawStyle (quadObjFill, GLU_FILL);
		gluQuadricDrawStyle (quadObjLine, GLU_LINE); 
		gluQuadricNormals (quadObjFill, GLU_SMOOTH);

		if(fov < 180.0)
		{
			glColor4f(1.0,1.0,0.0,0.2);
			gluCylinder(quadObjFill, r, 0.0, radius, 10, 2); 
			glColor4f(1.0,1.0,0.0,0.5);
			gluCylinder(quadObjLine, r, 0.0, radius, 10, 2); 
		}else
		{
			glColor4f(1.0,0.7,0.0,0.2);
			gluSphere(quadObjFill,radius,10,10);
			glColor4f(1.0,0.7,0.0,0.5);
			gluSphere(quadObjLine,radius,10,10);
		}
		
		gluDeleteQuadric(quadObjFill);
		gluDeleteQuadric(quadObjLine);
		
		glPopMatrix ();


		glPopMatrix();
		glPopMatrix();
	}

	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

void DebugView::selectNode()
{
	QString name = sender()->objectName();
	int NodeID = name.toInt();
	_main_application->_treeViewForm->selectByHandle(NodeID);
	_main_application->ui.m_sceneTreeWidget->selectNode(NodeID);
}

void DebugView::clear_light_buttons()
{
	for (int r = 0; r < _light_buttons.size(); ++r)
		_light_buttons.at(r)->deleteLater();
	_light_buttons.clear();
}
