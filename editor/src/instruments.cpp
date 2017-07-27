#include "instruments.h"
#include "editor.h"
InstrumentsManager::InstrumentsManager(QObject * parent) 
	: QObject(parent)
{
	_instrumentsManager_handle = this;
	//_instr_projection_buttons =		new ProjectionsButtons(this);
	//_instr_camera_navigation =		new CameraNavigation(this);
	_instr_select_object_by_mouse = new SelectObjectByMouse(this);
	//_instr_draw_area = new DrawArea(this);
	_instr_grid_view = new GridView(this);
	_instr_debug_view = new DebugView(this);
	m_isMouseRelease = true;
}

InstrumentsManager::~InstrumentsManager(void)
{

}

InstrumentsManager	* InstrumentsManager::get_instrument_manager_handle()
{
	return _instrumentsManager_handle;
}

void InstrumentsManager::render()
{
	
	//_instr_projection_buttons->render();
	//_instr_draw_area->render();
}

void InstrumentsManager::render_instruments(EngineGrView * egv)
{
	_instr_grid_view->render(egv);
	_instr_debug_view->render(egv);
	_instr_select_object_by_mouse->render_base();

	
	if(!egv->hasFocus())
		return;

	_instr_select_object_by_mouse->reconnect(egv);
	_instr_select_object_by_mouse->render();

	
	//render();
}

