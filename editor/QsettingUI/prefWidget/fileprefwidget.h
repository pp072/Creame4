#ifndef FILEPREFWIDGET_H
#define FILEPREFWIDGET_H

#include "pathprefwidget.h"

class filePrefWidget : public pathPrefWidget
{
public:
    filePrefWidget(QSettings *settings,const QString &group,const QString &nameKey,QFormLayout *layout);
 public slots:
    virtual void callback();
private:
	bool _is_rel_path_for_file;
};

#endif // FILEPREFWIDGET_H
