#ifndef XMLRPC_H
#define XMLRPC_H
#include <QXmlStreamWriter>
#include <QString>
#include <QVariant>
#include <QDomDocument>
#include <QTcpSocket>
#include "IRopeProject.h"

namespace Plugins
{
    class XMLRPC
    {
    public:
        static QString prepareRequest(const QString & methodName, const QVariantList & params);
        static void convertToXML(const QVariant & param, QString * type, QString * value);
        template<typename T>
        static T parseResponce(const QByteArray & data)
        {
            QDomDocument doc;
            QString errorMsg;
            int errorLine = 0;
            if (!doc.setContent(data, false, &errorMsg, &errorLine)){
                qDebug() << "error parse" << errorMsg << "line:" << errorLine;
                //qDebug() << data;
                return T();
            }
            QDomNode root = doc.documentElement();
            QDomNode node = root.namedItem("params");

            for(uint i = 0; i < node.childNodes().length(); ++i){
                QDomNode value = node.childNodes().at(i).firstChild();
                T ret;
                convertFromXML(value.firstChild().nodeName(), value.firstChild().firstChild(), &ret);
                return ret;
            }

            return T();
        }

        static QByteArray readSocket(QTcpSocket *soc);

    private:
        static void convertFromXML(const QString & type, const QDomNode & value, bool * ret);
        static void convertFromXML(const QString & type, const QDomNode & value, QString * ret);
        static void convertFromXML(const QString & type, const QDomNode & value, QList<ErrorItem> * ret);
        static void convertFromXML(const QString & type, const QDomNode & value, QStringList * ret);
        static void convertFromXML(const QString & type, const QDomNode & value, CompletitionList * ret);
        static void convertFromXML(const QString & type, const QDomNode & value, FindFile * ret);
    };
}
#endif // XMLRPC_H
