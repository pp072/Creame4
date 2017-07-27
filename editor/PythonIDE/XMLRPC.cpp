#include <QDebug>
#include "XMLRPC.h"

namespace Plugins {

void XMLRPC::convertFromXML(const QString & type, const QDomNode & value, bool * ret)
{
    *ret = type == "boolean" && value.nodeValue() == "1" ? true : false;
}

void XMLRPC::convertFromXML(const QString & type, const QDomNode & value, QString * ret)
{
    *ret = type == "string" && !value.nodeValue().isEmpty() ? value.nodeValue() : "";
}

void XMLRPC::convertFromXML(const QString & type, const QDomNode & value, QList<ErrorItem> * ret)
{
    if (type != "array")
        return;
    for(uint i = 0; i < value.childNodes().length(); ++i){
        QDomNode node = value.childNodes().at(i);
        //qDebug() << node.nodeName();
        if (node.firstChild().nodeName() == "struct"){
            ErrorItem err;
            for(uint j = 0; j < node.firstChild().childNodes().length(); ++j){
                QDomNode member = node.firstChild().childNodes().at(j);
                if (member.namedItem("name").firstChild().nodeValue() == "line")
                    err.lineNo = member.namedItem("value").firstChild().firstChild().nodeValue().toInt();
                if (member.namedItem("name").firstChild().nodeValue() == "message")
                    err.message = member.namedItem("value").firstChild().firstChild().nodeValue();
                if (member.namedItem("name").firstChild().nodeValue() == "type")
                    err.type = member.namedItem("value").firstChild().firstChild().nodeValue();
            }
            (*ret).append(err);
        }
    }
}

void XMLRPC::convertFromXML(const QString & type, const QDomNode & value, QStringList * ret)
{
    if (type != "array")
        return;
    for(uint i = 0; i < value.childNodes().length(); ++i){
        QString str;
        convertFromXML(value.childNodes().at(i).firstChild().nodeName(), value.childNodes().at(i).firstChild().firstChild(), & str);
        (*ret).append(str);
    }
}

void XMLRPC::convertFromXML(const QString & type, const QDomNode & value, CompletitionList * ret)
{
    if (type != "array")
        return;
    (*ret).word = value.firstChild().firstChild().firstChild().nodeValue();
    QStringList list;
    convertFromXML("array", value.firstChild().nextSibling().firstChild().firstChild(), &list);
    (*ret).items = list;
}

void XMLRPC::convertFromXML(const QString & /*type*/, const QDomNode & value, FindFile * ret)
{
    (*ret).file = value.firstChild().firstChild().firstChild().nodeValue();
    (*ret).offset = value.firstChild().nextSibling().firstChild().firstChild().nodeValue().toInt();
}

QString XMLRPC::prepareRequest(const QString & methodName, const QVariantList & params)
{
    QString data;
    QXmlStreamWriter out(&data);
    out.writeStartDocument();
    out.writeStartElement("methodCall");
    out.writeStartElement("methodName");
    out.writeCharacters(methodName);
    out.writeEndElement();
    out.writeStartElement("params");

    QString type, value;
    foreach(QVariant param, params){
        out.writeStartElement("param");
        out.writeStartElement("value");
        convertToXML(param, &type, &value);
        out.writeStartElement(type);
        out.writeCDATA(value);
        out.writeEndElement();
        out.writeEndElement();
        out.writeEndElement();
    }

    out.writeEndElement();
    out.writeEndElement();
    out.writeEndDocument();
    return data;
}

void XMLRPC::convertToXML(const QVariant & param, QString * type, QString * value)
{
    switch(param.type()){
    case QVariant::Int:
        *type = "int";
        *value = param.toString();
    case QVariant::Bool:
        *type = "boolean";
        *value = param.toBool() ? "1" : 0;
    default:
        *type = "string";
        *value = param.toString();
    }
}

QByteArray XMLRPC::readSocket(QTcpSocket *soc)
{
    ulong len = 0;
    soc->read((char*)&len, sizeof(unsigned long));
    qDebug() << "read" << len;

    while ((ulong)soc->bytesAvailable() < len){
        soc->waitForReadyRead();
    }

    QByteArray data = soc->read(len);
    qDebug() << data;
    soc->close();
    return data;
}

}
