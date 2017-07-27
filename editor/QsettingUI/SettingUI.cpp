#include <QDebug>
#include <QToolBox>
#include <QMessageBox>
#include <QStringList>
#include "SettingUI.h"
#include "ui_mainwindow.h"
#include "prefWidget/strprefwidget.h"
#include "prefWidget/intprefwidget.h"
#include "prefWidget/pathprefwidget.h"
#include "prefWidget/comboprefwidget.h"
#include "prefWidget/colorprefwidget.h"
#include "prefWidget/fontprefwidget.h"
#include "prefWidget/boolprefwidget.h"
#include "prefWidget/fileprefwidget.h"

SettingUI::SettingUI(QWidget *parent, QString settings_file) :
    QMainWindow(parent),
    ui(new Ui::SettingUI)
{
    ui->setupUi(this);
	_settings_file = settings_file;
	QSettings settings(_settings_file, QSettings::IniFormat );
	qDebug()<< "before build";
	buildUIFromQSetting(&settings);
	qDebug()<< "after build";
    QObject::connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(saveSettings()));

}

SettingUI::~SettingUI()
{
    delete ui;
}


void SettingUI::buildUIFromQSetting(QSettings *settings)
{
 QStringList lstGrp = settings->childGroups ();

 foreach(QString s,lstGrp)
 {
     QWidget *wid = new QWidget(ui->toolBox);
     QFormLayout *layout = new QFormLayout();
     wid->setLayout(layout);
     ui->toolBox->addTab(wid,s);
     settings->beginGroup(s);
     QStringList lstKeys = settings->childKeys();
	 qDebug()<< lstKeys;
     foreach(QString k,lstKeys)
     {
         IprefWidget *prefW=NULL;
         QStringList split = k.split("_");
         if (split.length()>1)
         {
			 QString pref_type = split[1];
             if (pref_type=="str")
             {
              prefW = new strPrefWidget(settings,s,k,layout);
             }
             if (pref_type=="int")
             {
                 prefW = new intPrefWidget(settings,s,k,layout);
             }
             if (pref_type=="path")
             {
                 prefW = new pathPrefWidget(settings,s,k,layout);
             }
             if (pref_type == "combo")
             {
                 prefW = new comboPrefWidget(settings,s,k,layout);
             }
             if (pref_type == "font")
             {
                 prefW = new fontPrefWidget(settings,s,k,layout);
             }
             if (pref_type == "color")
             {
                 prefW = new colorPrefWidget(settings,s,k,layout);
             }
             if (pref_type == "bool")
             {
                 prefW = new boolPrefWidget(settings,s,k,layout);
             }
             if (pref_type == "file")
             {
                 prefW = new filePrefWidget(settings,s,k,layout);
             }
         }

         if (prefW != NULL)
         {
           prefWidgetsList.append(prefW);
           //layout->addWidget((QWidget*)prefW);
       }
     }

     settings->endGroup();
 }
}

void SettingUI::saveSettings()
{
    qDebug()<< "saveSettings";
    QSettings settings(_settings_file, QSettings::IniFormat );
    bool hasError = false;
    for (int n=0;n<prefWidgetsList.length();n++ )
    {
        if (prefWidgetsList[n]->value().isValid() == false)
        {
            QMessageBox(QMessageBox::Critical, QString("Error"),QString("The entry ") + prefWidgetsList[n]->groupKey() + QString(" is invalide")).exec();
            hasError = true;
        }
    }
    if (hasError) return;

    for (int n=0;n<prefWidgetsList.length();n++ )
    {     
        settings.setValue( prefWidgetsList[n]->groupKey(), prefWidgetsList[n]->value());
    }
}
