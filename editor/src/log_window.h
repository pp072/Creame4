#ifndef LOG_WINDOW_H
#define LOG_WINDOW_H

#include <QMainWindow>
#include "ui_log_window.h"

class log_window : public QMainWindow
{
	Q_OBJECT

public:
	log_window(QWidget *parent = 0);
	~log_window();
	void clear();
	void updateLog(QListWidgetItem* item, bool scroll = true);

	void updateLogWindow(const QList<QListWidgetItem*>& items);
private:
	Ui::log_window ui;
	
private slots:
	void updateLog(const QList<QListWidgetItem*>& items);
};

#endif // LOG_WINDOW_H
