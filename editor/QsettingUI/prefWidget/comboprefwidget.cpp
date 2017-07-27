#include <QDebug>
#include <QComboBox>
#include <QStringList>
#include "comboprefwidget.h"

comboPrefWidget::comboPrefWidget(QSettings *setting,const QString &group,const QString &nameKey,QFormLayout *layout) : IprefWidget(setting,group,nameKey,layout)
{
    _combo = new QComboBox();

    if (setting->contains(nameKeyWithoutType()+ QString(".value")))
    {
      QStringList lst = setting->value(nameKeyWithoutType() + QString(".value")).toString().split(":");
      if (lst.length()>1)
      {
        foreach (QString s,lst)
        {
          _combo->addItem(s,s);
        }
        _combo->setCurrentIndex(setting->value(nameKey).toInt());
     }
      layout->addRow(_label,_combo);
  }
}


QVariant comboPrefWidget::value()
{
    QString result;
    result.setNum(_combo->currentIndex());
    return QVariant(result);
}
