#ifndef FONTPREFWIDGET_H
#define FONTPREFWIDGET_H

#include <QLabel>
#include <QFont>
#include "prefWidget/iprefwidget.h"

class fontPrefWidget: public IprefWidget
{
public:
    fontPrefWidget(QSettings *settings,const QString &group,const QString &nameKey,QFormLayout *layout);
    virtual QVariant value();
public slots:
   virtual void callback();

private:
   QFont _font;
   QLabel *_previewFont;
};

#endif // FONTPREFWIDGET_H
