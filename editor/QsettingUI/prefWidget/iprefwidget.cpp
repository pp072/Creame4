#include <QLabel>
#include <QGroupBox>
#include "iprefwidget.h"

IprefWidget::IprefWidget(QSettings *settings,const QString &group, const QString & nameKey,QFormLayout *layout) :
    QObject(0)
{
     _group = group;
     _nameKey = nameKey;   
     _label = new QLabel();
	 _setting = settings;
     QStringList lst = nameKey.split("_");
     if (lst.length()>0)
     {
       _label->setText(lst[2]);
     }
     else
     {
         _label->setText(nameKey);
     }

     //_layout->addWidget(label);
}

 QString IprefWidget::groupKey()
 {
     return _group+QString("/")+_nameKey;
 }

 void IprefWidget::callback()
 {

 }

 QString IprefWidget::nameKeyWithoutType()
 {
     QStringList lst = _nameKey.split("_");
     if (lst.length()>0)
     {
         return lst[2];
     }
     return _nameKey;
 }

 bool IprefWidget::convertStrToBool(const QString &val)
 {
     if (val == "true" || val == "TRUE") return true;
     return false;
 }
 void IprefWidget::addRowToLayout( QFormLayout * layout , QWidget *widget )
 {
	 if (_setting->contains(nameKeyWithoutType()+ QString(".group")))
	 {
		 QString group_name = _setting->value(nameKeyWithoutType()+ QString(".group")).toString();
		 QWidget *wid = layout->parentWidget();
		 QGroupBox *group_box= wid->findChild<QGroupBox *>(group_name);
		 QHBoxLayout *hbox = new QHBoxLayout;
		 QLabel *label = new QLabel(_label->text());
		 label->setMinimumSize(100,10);
		 if(group_box){
			 QVBoxLayout *vbox = (QVBoxLayout*)group_box->layout();
			 vbox->insertLayout(0,hbox,0);
			 hbox->addWidget(label);
			 //hbox->addStretch(1);
			 hbox->addWidget(widget);
		 }
		 else{
			 QGroupBox * group_box = new QGroupBox(group_name);
			 group_box->setObjectName(group_name);
			 QVBoxLayout *vbox = new QVBoxLayout;
			 vbox->addStretch(1);
			 group_box->setLayout(vbox);
			 vbox->insertLayout(0,hbox);
			 hbox->addWidget(label);
			 //hbox->addStretch(1);
			 hbox->addWidget(widget);
			 layout->addRow(group_box);
		 }

	 }
	 else{
		 QFrame *frame = new QFrame ();
		 QHBoxLayout *hbox = new QHBoxLayout();
		 frame->setLayout(hbox);
		 //hbox->addStretch(1);
		 hbox->addWidget(widget);
		 layout->addRow(_label,frame);
	 }
 }