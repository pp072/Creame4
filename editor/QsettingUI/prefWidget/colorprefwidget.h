#ifndef COLORPREFWIDGET_H
#define COLORPREFWIDGET_H

#include <QColor>
#include <QLabel>
#include "colorlisteditor.h"
#include "prefWidget/iprefwidget.h"

class colorPrefWidget: public IprefWidget
{
public:
    colorPrefWidget(QSettings *settings,const QString &group,const QString &nameKey,QFormLayout *layout);
    virtual QVariant value();
private:

   ColorListEditor *_comboColor;
};

#endif // COLORPREFWIDGET_H
