#ifndef COMBOPREFWIDGET_H
#define COMBOPREFWIDGET_H


#include "prefWidget/iprefwidget.h"

class QComboBox;

class comboPrefWidget : public IprefWidget
{
public:
    comboPrefWidget(QSettings *settings,const QString &group,const QString &nameKey,QFormLayout *layout);
    virtual QVariant value();
protected:
    QComboBox *_combo;
    QString _setValue;
};

#endif // COMBOPREFWIDGET_H
