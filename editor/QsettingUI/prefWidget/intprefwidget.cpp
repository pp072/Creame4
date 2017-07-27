#include "intprefwidget.h"

#include <QSpinBox>
intPrefWidget::intPrefWidget(QSettings *setting,const QString &group,const QString &nameKey,QFormLayout *layout) : IprefWidget(setting,group,nameKey,layout)
{
    _spinBox = new QSpinBox();
    _spinBox->setValue(setting->value(nameKey).toInt());
      layout->addRow(_label,_spinBox);

      if (setting->contains(nameKeyWithoutType()+ QString(".min")))
      {
          _spinBox->setMinimum(setting->value(nameKeyWithoutType()+ QString(".min")).toInt());

      }
      if (setting->contains(nameKeyWithoutType()+ QString(".max")))
      {
           _spinBox->setMaximum(setting->value(nameKeyWithoutType()+ QString(".max")).toInt());
      }
}


QVariant intPrefWidget::value()
{
    QVariant result;
    result.setValue(_spinBox->value());
    return result;
}
