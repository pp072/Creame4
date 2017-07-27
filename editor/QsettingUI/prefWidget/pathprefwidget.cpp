#include <QDebug>
#include <QToolButton>
#include <QFile>
#include <QFileDialog>
#include "pathprefwidget.h"

pathPrefWidget::pathPrefWidget(QSettings *setting,const QString &group,const QString &nameKey,QFormLayout *layout) :  IprefWidget(setting,group,nameKey,layout)
{
    QFrame *frame = new QFrame ();
    QHBoxLayout *hbox = new QHBoxLayout();
    frame->setLayout(hbox);
    edit = new QLineEdit();
    edit->setText(setting->value(nameKey).toString());
    hbox->addWidget(edit);
    QToolButton *button= new QToolButton();
    button->setText("...");
    hbox->addWidget(button);
      QObject::connect(button,SIGNAL(clicked()),(pathPrefWidget *)this,SLOT(callback()));    
    //layout->addRow(_label,frame);
	frame->layout()->setMargin(0);
	addRowToLayout(layout,frame);

    pathMustExist = false;
    if (setting->contains(nameKeyWithoutType()+ QString(".pathMustExist")))
    {
      pathMustExist = convertStrToBool(setting->value(nameKeyWithoutType()+ QString(".pathMustExist")).toString());
    }

}

void pathPrefWidget::callback()
{
    QString filename = QFileDialog::getExistingDirectory(0,tr("choose path"),edit->text());
    if (filename.isEmpty() == false)
    {
        edit->setText(filename);
    }
}

QVariant pathPrefWidget::value()
{
    QVariant result;

    if (pathMustExist)
    {
      if (QFile::exists(edit->text()))
      {
        result.setValue(edit->text());
        return result;
      }
      else
      {
        return result;
      }
  }
    else
    {
        result.setValue(edit->text());
        return result;
    }
}
