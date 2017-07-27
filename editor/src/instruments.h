#ifndef instruments_h__
#define instruments_h__
#include <QObject>
//#include "projectionsbuttons.h"
//#include "cameranavigation.h"
#include "selectobjectbymouse.h"
#include "gridview.h"
#include "debugview.h"
//#include "drawarea.h"
static class InstrumentsManager *_instrumentsManager_handle;
class InstrumentsManager : public QObject
{
	Q_OBJECT
public:

	InstrumentsManager(QObject *parent);
	~InstrumentsManager(void);
	static InstrumentsManager	*get_instrument_manager_handle();
	SelectObjectByMouse * getInstr_select_object_by_mouse() const { return _instr_select_object_by_mouse; }
	//ProjectionsButtons	*_instr_projection_buttons;
	//CameraNavigation	*_instr_camera_navigation;
	SelectObjectByMouse *_instr_select_object_by_mouse;
	GridView			*_instr_grid_view;
	DebugView			*_instr_debug_view;

	//DrawArea			*_instr_draw_area;
	void render();
	volatile  bool m_isMouseRelease;
private slots:
	void render_instruments(EngineGrView *);
	

};
#endif // instruments_h__
