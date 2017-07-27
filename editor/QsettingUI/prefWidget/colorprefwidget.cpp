#include <QDebug>
#include <QToolButton>
#include <QColorDialog>
#include "colorprefwidget.h"

colorPrefWidget::colorPrefWidget(QSettings *setting,const QString &group,const QString &nameKey,QFormLayout *layout) : IprefWidget(setting,group,nameKey,layout)
{  
    _comboColor = new ColorListEditor();
    layout->addRow(_label,_comboColor);
}

QVariant colorPrefWidget::value()
{
     return QVariant(_comboColor->color().name());
}

