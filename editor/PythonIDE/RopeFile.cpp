#include <QTcpSocket>
#include "RopeFile.h"
#include "RopeProject.h"
#include "XMLRPC.h"

namespace Plugins {

RopeFile::RopeFile(const QString & fileName, RopeProject * prj):
    _fileName(fileName),
    _prj(prj)
{
}

RopeFile::~RopeFile()
{
}

void RopeFile::errorsList()
{
    QVariantList list;
    list << _fileName;
    _prj->makeRequest("errorsList", list, this, SLOT(onErrorList()));
}

void RopeFile::autocompletitionList(const QString& text, int position)
{
    QVariantList list;
    list << _fileName;
    list << text;
    list << position;
    _prj->makeRequest("autocompletitionList", list, this, SLOT(onAutoComplite()));
}

void RopeFile::onErrorList()
{
    QList<ErrorItem> list = XMLRPC::parseResponce< QList<ErrorItem> >(XMLRPC::readSocket(qobject_cast<QTcpSocket*>(sender())));
    emit errorListComplete(list);
}

void RopeFile::onAutoComplite()
{
    CompletitionList list = XMLRPC::parseResponce<CompletitionList>(XMLRPC::readSocket(qobject_cast<QTcpSocket*>(sender())));
	//qDebug() << list.word <<" : " <<list.items;
    emit autoComplite(list);
}

void RopeFile::findImplementation(const QString& text, int position)
{
    QVariantList list;
    list << _fileName;
    list << text;
    list << position;
    _prj->makeRequest("findImplementation", list, this, SLOT(onFindImplementation()));
}

void RopeFile::onFindImplementation()
{
    FindFile find = XMLRPC::parseResponce<FindFile>(XMLRPC::readSocket(qobject_cast<QTcpSocket*>(sender())));
    emit findImplementationComplite(find.file, find.offset);
}

}
