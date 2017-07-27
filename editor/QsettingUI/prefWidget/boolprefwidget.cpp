#include "boolprefwidget.h"
#include <QDebug>
#include <QGroupBox>
boolPrefWidget::boolPrefWidget(QSettings *setting,const QString &group,const QString &nameKey,QFormLayout *layout) : IprefWidget(setting,group,nameKey,layout)
{
    checkBox = new QCheckBox();
	checkBox->setChecked(convertStrToBool(setting->value(nameKey).toString()));


	QFrame *frame = new QFrame ();
	QHBoxLayout *hbox = new QHBoxLayout();
	frame->setLayout(hbox);
	hbox->addStretch(1);
	hbox->addWidget(checkBox);
	frame->layout()->setMargin(0);
	addRowToLayout(layout, frame);
}

QVariant boolPrefWidget::value()
{
    if (checkBox->isChecked())
    {
        return QVariant("true");
    }
    else
    {
        return QVariant("false");
    }
}


