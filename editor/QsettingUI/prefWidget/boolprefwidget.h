#ifndef BOOLPREFWIDGET_H
#define BOOLPREFWIDGET_H
#include <QCheckBox>
#include <QHBoxLayout>
#include "prefWidget/iprefwidget.h"

class boolPrefWidget : public IprefWidget
{
public:
    boolPrefWidget(QSettings *settings,const QString &group,const QString &nameKey,QFormLayout *layout);

	

	virtual QVariant value();

private:
    QCheckBox *checkBox;
};

#endif // BOOLPREFWIDGET_H
