#ifndef RCWINDOW_H
#define RCWINDOW_H

#include <QMainWindow>
#include "GeneratedFiles/ui_rcwindow.h"
#include "enginegrview.h"

#ifdef DLL_NONCLIENT_BUILD // в директивах препроцессора проекта с dll нужно прописать DLL_NONCLIENT_BUILD
#   define DLL_EXPORT __declspec(dllexport)
#else
#   define DLL_EXPORT __declspec(dllimport)
#endif
class RCWinMan;
class DLL_EXPORT RCWindow : public QMainWindow
{
	Q_OBJECT
	friend RCWinMan;
public:
	RCWindow(RCWinMan *parent = 0, bool isPrimeWindow = false);
	~RCWindow();

	
	EngineGrView * getEngineGrView() const { return _engineGrView; }
	RCWinMan * getRc_win_man() const;
	void createWind();
	void setCamera(int cam);
	int getCamera();
	void set_title(QString);
	Ui::RCWindow ui;
	QDockWidget * _dockWidget;
private:
	
	EngineGrView *_engineGrView;
	RCWinMan *_rc_win_man;
};

#endif // RCWINDOW_H
