#ifndef RCWINMAN_H
#define RCWINMAN_H

#include <QObject>
#include "rcwindow.h"


class DLL_EXPORT RCWinMan : public QObject
{
	Q_OBJECT

public:
	RCWinMan(QWidget *parent);
	~RCWinMan();
	RCWindow * getPrimeWindow() const { return _primeWindow; }
	RCWindow * getActiveWindow();
	RCWindow * createNewWindow(QWidget *parent = 0);
//	bool loadScene();
	unsigned int getSelected_item() const { return _selected_item; }
	void update();
private:
	RCWindow *    _primeWindow;
	std::vector <RCWindow*>	_windows;
	unsigned int	_selected_item;
	
	

private slots:
	void render();
	void slot_get_item_selected(unsigned int);
	void updateLog();
signals:
	void logMessages(const QList<QListWidgetItem*>&);
};

#endif // RCWINMAN_H
