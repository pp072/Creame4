#include "filestabwidget.h"

FilesTabWidget::FilesTabWidget(QWidget *parent)
	: QTabWidget(parent)
{
	setTabsClosable(true);
//	connect(this,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));
}

FilesTabWidget::~FilesTabWidget()
{

}

void FilesTabWidget::closeTab( int index )
{
//	delete this->widget(index);
//	this->removeTab(index);
}
