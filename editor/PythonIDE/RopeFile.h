#ifndef ROPEFILE_H
#define ROPEFILE_H
#include <QString>
#include <QLocalSocket>
#include "IRopeProject.h"


namespace Plugins
{
    class RopeProject;
    class RopeFile: public IRopeFile
    {
        Q_OBJECT
    public:
        RopeFile(const QString & fileName, RopeProject * _prj);
        virtual ~RopeFile();

   public slots:
        virtual void errorsList();
        virtual void autocompletitionList(const QString& text, int position);
        virtual void findImplementation(const QString& text, int position);
    private slots:
        void onErrorList();
        void onAutoComplite();
        void onFindImplementation();
    private:
        QString _fileName;
        RopeProject *_prj;
    };
}

#endif // ROPEFILE_H
