#include <QLineEdit>
#include "strprefwidget.h"

strPrefWidget::strPrefWidget(QSettings *setting,const QString &group,const QString &nameKey,QFormLayout *layout) : IprefWidget(setting,group,nameKey,layout)
{
    edit = new QLineEdit();
    edit->setText(setting->value(nameKey).toString());
    layout->addRow(_label,edit);
    if (setting->contains(nameKeyWithoutType()+ QString(".regexp")))
    {
      _regexp = QRegExp(setting->value(nameKeyWithoutType()+ QString(".regexp")).toString());
    }
}

QVariant strPrefWidget::value()
{
    QVariant result;

    if (_regexp.isEmpty() == false)
    {
        if (_regexp.exactMatch(edit->text()) == false)
        {
            return result;
        }
    }
    result.setValue(edit->text());
    return result;
}

