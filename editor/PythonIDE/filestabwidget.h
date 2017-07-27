#ifndef FILESTABWIDGET_H
#define FILESTABWIDGET_H

#include <QTabWidget>

class FilesTabWidget : public QTabWidget
{
	Q_OBJECT

public:
	FilesTabWidget(QWidget *parent);
	~FilesTabWidget();

private:
public slots:
	void closeTab(int index);
	
};

#endif // FILESTABWIDGET_H
