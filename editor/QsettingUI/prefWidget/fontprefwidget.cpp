#include <QDebug>
#include <QToolButton>
#include <QFontDialog>
#include "fontprefwidget.h"

fontPrefWidget::fontPrefWidget(QSettings *setting,const QString &group,const QString &nameKey,QFormLayout *layout) : IprefWidget(setting,group,nameKey,layout)
{
    QFrame *frame = new QFrame ();
    QHBoxLayout *hbox = new QHBoxLayout();
    frame->setLayout(hbox);

    _font.fromString(setting->value(nameKey).toString());
    _previewFont = new QLabel();
    _previewFont->setText(_font.family());
     _previewFont->setFont(_font);

    hbox->addWidget(_previewFont);

    QToolButton *button= new QToolButton();
    button->setText("...");
    hbox->addWidget(button);
      QObject::connect(button,SIGNAL(clicked()),(fontPrefWidget *)this,SLOT(callback()));
    layout->addRow(_label,frame);
}
QVariant fontPrefWidget::value()
{    
  return QVariant(_font.toString());
}

void fontPrefWidget::callback()
{
    bool ok = false;
    QFontDialog * fontDialog = new QFontDialog(_font);
    _font = fontDialog->getFont(&ok);
    if (ok == true)
    {
       _previewFont->setFont(_font);
       _previewFont->setText(_font.family());
    }
}
