#ifndef PATHPREFWIDGET_H
#define PATHPREFWIDGET_H

#include <QLineEdit>
#include "prefWidget/iprefwidget.h"

class pathPrefWidget : public  IprefWidget
{
public:
   explicit pathPrefWidget(QSettings *settings,const QString &group,const QString &nameKey,QFormLayout *layout);
   virtual QVariant value();
public slots:
   virtual void callback();
protected:
   QLineEdit *edit;
   bool pathMustExist;
};

#endif // PATHPREFWIDGET_H
