#ifndef IROPEPROJECT_H
#define IROPEPROJECT_H
#include <QString>
#include <QMetaType>
#include <QStringList>

namespace Plugins
{
    struct ErrorItem {
        int     lineNo;
        QString message;
        QString type;
    };
    struct CompletitionList {
        QString word;
        QStringList items;
    };
    struct FindFile {
        QString file;
        int offset;
    };

    class IRopeFile : public QObject
    {
        Q_OBJECT
    public:
        virtual ~IRopeFile(){}
    public slots:
        virtual void errorsList() = 0;
        virtual void autocompletitionList(const QString& text, int position) = 0;
        virtual void findImplementation(const QString& text, int position) = 0;
    signals:
        void errorListComplete(const QList<ErrorItem>&);
		void autoComplite(const Plugins::CompletitionList&);
        void findImplementationComplite(const QString &, int);
    };

    class IRopeProject : public QObject {
        Q_OBJECT
    public:
        virtual ~IRopeProject(){}
        virtual IRopeFile* openFile(const QString & fileName) = 0;
        virtual void closeFile(const QString & fileName) = 0;
        virtual void revalidate(const QString & path, const QString & mainScript, const QString& projectPath) = 0;
        virtual void classNames(const QString & prefix) = 0;
    };
}
Q_DECLARE_METATYPE(Plugins::ErrorItem)
Q_DECLARE_METATYPE(QList<Plugins::ErrorItem>)

#endif
