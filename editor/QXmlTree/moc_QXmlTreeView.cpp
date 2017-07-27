/****************************************************************************
** Meta object code from reading C++ file 'QXmlTreeView.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "QXmlTreeView.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QXmlTreeView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QXmlTreeView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   14,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      53,   13,   13,   13, 0x0a,
      71,   13,   13,   13, 0x0a,
      91,   83,   13,   13, 0x0a,
     115,   13,   13,   13, 0x2a,
     152,  135,   13,   13, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_QXmlTreeView[] = {
    "QXmlTreeView\0\0node\0currentNodeChanged(QXmlTreeNode*)\0"
    "copyCurrentNode()\0pasteNode()\0confirm\0"
    "removeCurrentNode(bool)\0removeCurrentNode()\0"
    "current,previous\0"
    "currentNodeChanged(QModelIndex,QModelIndex)\0"
};

void QXmlTreeView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QXmlTreeView *_t = static_cast<QXmlTreeView *>(_o);
        switch (_id) {
        case 0: _t->currentNodeChanged((*reinterpret_cast< QXmlTreeNode*(*)>(_a[1]))); break;
        case 1: _t->copyCurrentNode(); break;
        case 2: _t->pasteNode(); break;
        case 3: _t->removeCurrentNode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->removeCurrentNode(); break;
        case 5: _t->currentNodeChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QXmlTreeView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QXmlTreeView::staticMetaObject = {
    { &QTreeView::staticMetaObject, qt_meta_stringdata_QXmlTreeView,
      qt_meta_data_QXmlTreeView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QXmlTreeView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QXmlTreeView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QXmlTreeView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QXmlTreeView))
        return static_cast<void*>(const_cast< QXmlTreeView*>(this));
    return QTreeView::qt_metacast(_clname);
}

int QXmlTreeView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void QXmlTreeView::currentNodeChanged(QXmlTreeNode * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
