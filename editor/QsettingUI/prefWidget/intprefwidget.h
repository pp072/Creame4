#ifndef INTPREFWIDGET_H
#define INTPREFWIDGET_H


#include "prefWidget/iprefwidget.h"

class QSpinBox;

class intPrefWidget : public IprefWidget
{
public:
    intPrefWidget(QSettings *settings,const QString &group,const QString &nameKey,QFormLayout *layout);
    virtual QVariant value();
private:
    QSpinBox *_spinBox;
};

#endif // INTPREFWIDGET_H
