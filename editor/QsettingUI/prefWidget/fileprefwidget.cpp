#include <QDebug>
#include <QFile>
#include <QDir>
#include <QFileDialog>

#include "fileprefwidget.h"

filePrefWidget::filePrefWidget(QSettings *setting,const QString &group,const QString &nameKey,QFormLayout *layout) :  pathPrefWidget(setting,group,nameKey,layout), _is_rel_path_for_file(false)
{
	if (setting->contains(nameKeyWithoutType()+ QString(".relative")))
		_is_rel_path_for_file = convertStrToBool(setting->value(nameKeyWithoutType()+ QString(".relative")).toString());;
}


void filePrefWidget::callback()
{
    QString filename = QFileDialog::getOpenFileName(0,tr("choose file"),edit->text());
    if (filename.isEmpty() == false)
    {
		if(_is_rel_path_for_file)
		{
			QDir file_path;
			filename = file_path.relativeFilePath(filename);
		}
		edit->setText(filename);
    }
}
