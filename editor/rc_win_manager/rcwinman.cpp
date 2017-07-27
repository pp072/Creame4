#include "rcwinman.h"
#include "Horde3DUtils.h"
#define stop __asm nop
RCWinMan::RCWinMan(QWidget *parent)
	: QObject(parent),_selected_item(0)
{
	_primeWindow = new RCWindow(this, true);
	_primeWindow->ui.actionAlwaysUpdate->setChecked(true);
	_windows.push_back(_primeWindow);
}

RCWinMan::~RCWinMan()
{
	h3dutDumpMessages();
	h3dRelease();
}

RCWindow * RCWinMan::createNewWindow( QWidget *parent /*= 0*/ )
{
	RCWindow *rcwin = new RCWindow(this);
	_windows.push_back(rcwin);


	rcwin->getEngineGrView()->getQglWidget()->makeCurrent();
	HGLRC curcont =  wglGetCurrentContext();
	bool ishared = wglShareLists(_primeWindow->getEngineGrView()->_CrHGLRC, curcont);
	rcwin->createWind();


	return rcwin;
}

void RCWinMan::render()
{
	for(int i = 0; i < _windows.size();i++)
	{
		if(_windows[i]->getEngineGrView()->hasFocus() || _windows[i]->ui.actionAlwaysUpdate->isChecked())
		{
			_windows[i]->getEngineGrView()->viewport()->update();
			updateLog();
		}
	}
}

void RCWinMan::slot_get_item_selected( unsigned int item)
{

	_selected_item = item;

}

RCWindow * RCWinMan::getActiveWindow()
{
	for(int i = 0; i < _windows.size();i++)
	{
		if(_windows[i]->hasFocus())
			return _windows[i];
	}
}

void RCWinMan::updateLog()
{
	int level;
	float time;
	QList<QListWidgetItem*> items;
	QString message;
	while(!(message = h3dGetMessage(&level, &time)).isEmpty())
	{
		// Create List Widget Item (removing HTML Tags from message since Horde3D still adds some tags to the warnings)
		QListWidgetItem* item = new QListWidgetItem(QString::number(time) + "\t" + message.remove(QRegExp("(<(\\s*)?[a-z/]+(\\s*)?(/)?>)",Qt::CaseInsensitive, QRegExp::RegExp2)), 0, level);
		items.push_back(item);		
	}
	if (!items.isEmpty())
		emit logMessages(items);
}

void RCWinMan::update()
{
	for(int i = 0; i < _windows.size();i++)
	{
// 		if(_windows[i]->getEngineGrView()->hasFocus() || _windows[i]->ui.actionAlwaysUpdate->isChecked())
// 		{
			_windows[i]->getEngineGrView()->viewport()->update();
//			updateLog();
//		}
	}
}

// bool RCWinMan::loadScene()
// {
// 	return false;
// }