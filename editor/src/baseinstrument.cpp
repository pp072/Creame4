#include "baseinstrument.h"
#include "editor.h"
#include "instruments.h"

#define stop __asm nop
BaseInstrument::BaseInstrument( QObject *parent, int type  )
: QObject(parent),  ui(editor::get_main_window_handle()->ui), _type(type),_egv(0x0)
{
	_main_application = editor::get_main_window_handle();
	_instruments_manager = InstrumentsManager::get_instrument_manager_handle();
	reconnect(_main_application->_rcWinMan->getPrimeWindow()->getEngineGrView());
}

void BaseInstrument::reconnect(EngineGrView * egv)
{
	if(egv == _egv)
		return;
	if(_egv)
	{
		disconnect(SIGNAL(signal_emit_MouseMoveEvent(QMouseEvent *)),_egv);
		disconnect(SIGNAL(signal_emit_MouseReleaseEvent(QMouseEvent *)),_egv);
		disconnect(SIGNAL(signal_emit_MouseWheelEvent(QMouseEvent *)),_egv);
		disconnect(SIGNAL(signal_emit_MousePressEvent(QMouseEvent *)),_egv);
	}
	_egv = egv;
	connect(_egv,SIGNAL(signal_emit_MouseMoveEvent(QMouseEvent *)), this,SLOT(slot_emit_MouseMoveEventEngineGrView(QMouseEvent *)));
	connect(_egv,SIGNAL(signal_emit_MouseReleaseEvent( QMouseEvent * )), this,SLOT(slot_emit_MouseReleaseEventEngineGrView( QMouseEvent * )));
	connect(_egv,SIGNAL(signal_emit_MouseWheelEvent(QWheelEvent *)), this,SLOT(slot_emit_MouseWheelEventEngineGrView(QWheelEvent *)));
	connect(_egv,SIGNAL(signal_emit_MousePressEvent(QMouseEvent *)), this,SLOT(slot_emit_MousePressEventEngineGrView(QMouseEvent *)));
}

BaseInstrument::~BaseInstrument()
{

}

void BaseInstrument::slot_emit_MouseMoveEventEngineGrView( QMouseEvent *event )
{

}

void BaseInstrument::slot_emit_MouseReleaseEventEngineGrView( QMouseEvent *event )
{
	//m_isMouseRelease = true;
	_instruments_manager->m_isMouseRelease = true;
	m_MouseButton = 0x0;
}

void BaseInstrument::slot_emit_MouseWheelEventEngineGrView( QWheelEvent *event )
{
	;
}

void BaseInstrument::slot_emit_MousePressEventEngineGrView( QMouseEvent *event )
{
	switch(event->button())
	{
	case Qt::LeftButton:
		m_MouseButton=Qt::LeftButton;
		_instruments_manager->m_isMouseRelease = false;
		break;
	case Qt::RightButton:
		m_MouseButton=Qt::RightButton;
		_instruments_manager->m_isMouseRelease = false;
		break;
	case Qt::MidButton:
		m_MouseButton=Qt::MidButton;
		_instruments_manager->m_isMouseRelease = false;
		break;
	}
	//m_isMouseRelease = false;
}
