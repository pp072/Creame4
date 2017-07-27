#include "rcwindow.h"
#include "rcwinman.h"
RCWindow::RCWindow(RCWinMan *parent ,bool isPrimeWindow)
	: QMainWindow(0),_dockWidget(0x0)
{
	_rc_win_man = parent;
	ui.setupUi(this);
	_engineGrView = new EngineGrView(this,isPrimeWindow);
	_engineGrView->_rcwindow = this;
	_engineGrView->_camera_nav = new CameraNavigation(_engineGrView);
	ui._layout->addWidget(_engineGrView);
	
	
}

RCWindow::~RCWindow()
{

}
void RCWindow::createWind()
{
	_engineGrView->initWindow();
}

void RCWindow::setCamera( int cam )
{
	_engineGrView->setCam( (H3DNode)cam);
	
}

int RCWindow::getCamera()
{
	return _engineGrView->getCam();
}

RCWinMan * RCWindow::getRc_win_man() const
{
	return _rc_win_man;
}

void RCWindow::set_title( QString str)
{
	if(_dockWidget)
		_dockWidget->setWindowTitle(str);
}
