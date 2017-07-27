#ifndef BASEINSTRUMENT_H
#define BASEINSTRUMENT_H

#include <QObject>
#include <QMouseEvent>
#include "enginegrview.h"
class InstrumentsManager;
class editor;
namespace Ui
{
	class editorClass;
}
struct GuiInstrumentsType
{
	enum List
	{
		Undefined = 0,
		ProjectionButtons,
		CameraNavigation,
		SelectObjectByMouse,
		Grid,
		DebugView
	};
};
class BaseInstrument : public QObject
{
	friend InstrumentsManager;
	Q_OBJECT

public:
	BaseInstrument(QObject *parent, int type );
	~BaseInstrument();
	void reconnect(EngineGrView * egv);
protected:
	Ui::editorClass &ui;
	editor				*_main_application;
	InstrumentsManager	*_instruments_manager;
	int					_type;
	EngineGrView *		_egv;
	//bool			m_isMouseRelease;
	int				m_MouseButton;
private slots:
	virtual void slot_emit_MouseMoveEventEngineGrView( QMouseEvent *event );
	virtual void slot_emit_MouseReleaseEventEngineGrView( QMouseEvent *event );
	virtual void slot_emit_MousePressEventEngineGrView( QMouseEvent *event );
	virtual void slot_emit_MouseWheelEventEngineGrView(QWheelEvent *event);
};

#endif // BASEINSTRUMENT_H
