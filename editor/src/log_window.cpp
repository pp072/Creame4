#include "log_window.h"

log_window::log_window(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->statusBar()->hide();
}

log_window::~log_window()
{

}
void log_window::updateLog(QListWidgetItem* item, bool scroll /* = true*/)
{	
	Q_ASSERT(item);
	ui.m_logWidget->addItem(item);
	if(item->text().contains("GE_"))
	{
		item->setBackgroundColor(QColor("#E1E1E1"));
		item->setText(item->text().remove("GE_"));
	}
	switch( item->type() )
	{
	case 1:
		item->setText(tr("Error: ") + item->text());
		item->setTextColor(QColor("#EE1100"));	
		//item->setHidden(!m_actionLogShowErrors->isChecked());
		break;
	case 2:
		item->setText(tr("Warning: ") + item->text());
		item->setTextColor(QColor("#FFCC00"));
		//item->setHidden(!m_actionLogShowWarnings->isChecked());
		break;
	case 3:
		//item->setTextColor(QColor("#C0C0C0"));
		//item->setHidden(!m_actionLogShowInfo->isChecked());
		break;
	case 4:
		item->setTextColor(QColor("#CC8080"));
		//item->setHidden(!m_actionLogShowDebug->isChecked());
		break;
	}
	if( scroll ) ui.m_logWidget->scrollToBottom();
}

void log_window::updateLog( const QList<QListWidgetItem*>& items )
{
	QList<QListWidgetItem*>::const_iterator iter = items.constBegin();
	while (iter != items.constEnd())
	{	
		updateLog(*iter, false);
			++iter;
	}
	ui.m_logWidget->scrollToBottom();
}

void log_window::clear()
{
	ui.m_logWidget->clear();
}

void log_window::updateLogWindow( const QList<QListWidgetItem*>& items )
{
	updateLog(items);
}
