#ifndef STRPREFWIDGET_H
#define STRPREFWIDGET_H

#include <QRegExp>
#include <QLineEdit>
#include "prefWidget/iprefwidget.h"



class strPrefWidget : public IprefWidget
{
public:
    strPrefWidget(QSettings *settings,const QString &group,const QString &nameKey,QFormLayout *layout);
     virtual QVariant value();
protected:

     QRegExp _regexp;
    QLineEdit *edit;

};

#endif // STRPREFWIDGET_H
