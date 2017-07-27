/****************************************************************************
** Meta object code from reading C++ file 'designer.h'
**
** Created: Sat 15. Mar 14:54:08 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../designer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'designer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Designer[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,
      28,    9,    9,    9, 0x05,
      48,    9,    9,    9, 0x05,
      68,    9,    9,    9, 0x05,
      88,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
     109,    9,    9,    9, 0x08,
     127,    9,    9,    9, 0x08,
     146,    9,    9,    9, 0x08,
     164,    9,    9,    9, 0x08,
     180,    9,    9,    9, 0x08,
     205,    9,    9,    9, 0x08,
     223,    9,    9,    9, 0x08,
     241,    9,    9,    9, 0x08,
     259,    9,    9,    9, 0x08,
     283,    9,  278,    9, 0x08,
     294,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Designer[] = {
    "Designer\0\0fileChanged(bool)\0"
    "copyAvailable(bool)\0undoAvailable(bool)\0"
    "redoAvailable(bool)\0pasteAvailable(bool)\0"
    "onSwitchWidgets()\0onSwitchTabOrder()\0"
    "onSwitchBuddies()\0onFormChanged()\0"
    "closeEvent(QCloseEvent*)\0onUndoAvailable()\0"
    "onRedoAvailable()\0onCopyAvailable()\0"
    "onPasteAvailable()\0bool\0saveFile()\0"
    "previewForm()\0"
};

void Designer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Designer *_t = static_cast<Designer *>(_o);
        switch (_id) {
        case 0: _t->fileChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->copyAvailable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->undoAvailable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->redoAvailable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->pasteAvailable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->onSwitchWidgets(); break;
        case 6: _t->onSwitchTabOrder(); break;
        case 7: _t->onSwitchBuddies(); break;
        case 8: _t->onFormChanged(); break;
        case 9: _t->closeEvent((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        case 10: _t->onUndoAvailable(); break;
        case 11: _t->onRedoAvailable(); break;
        case 12: _t->onCopyAvailable(); break;
        case 13: _t->onPasteAvailable(); break;
        case 14: { bool _r = _t->saveFile();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 15: _t->previewForm(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Designer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Designer::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Designer,
      qt_meta_data_Designer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Designer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Designer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Designer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Designer))
        return static_cast<void*>(const_cast< Designer*>(this));
    return QWidget::qt_metacast(_clname);
}

int Designer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void Designer::fileChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Designer::copyAvailable(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Designer::undoAvailable(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Designer::redoAvailable(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Designer::pasteAvailable(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
