#ifndef IPREFWIDGET_H
#define IPREFWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QSettings>
#include <QFormLayout>




class IprefWidget : public QObject
{
    Q_OBJECT
public:
    explicit IprefWidget(QSettings *settings,const QString &group, const QString & nameKey,QFormLayout *layout);

    QString groupKey();
    virtual QVariant value() =0;

public slots:
    virtual void callback();
protected :
        QString nameKeyWithoutType();
        bool convertStrToBool(const QString &val);
		void addRowToLayout( QFormLayout * layout , QWidget *widget );
		QString _group;
		QString _nameKey;
		QLabel *_label;
		QSettings *_setting;
};

#endif // IPREFWIDGET_H
