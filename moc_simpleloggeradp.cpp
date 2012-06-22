/****************************************************************************
** Meta object code from reading C++ file 'simpleloggeradp.h'
**
** Created: Mon Apr 16 10:26:56 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "simpleloggeradp.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'simpleloggeradp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SimpleLoggerADP[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      43,   17,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
     151,  110,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SimpleLoggerADP[] = {
    "SimpleLoggerADP\0\0formattedMessage,category\0"
    "formattedMessage(QString,SimpleLoggerRoutingInfo::MessageCategory)\0"
    "message,location,dateTime,category,level\0"
    "receiveMessage(QString,QString,QDateTime,SimpleLoggerRoutingInfo::Mess"
    "ageCategory,int)\0"
};

void SimpleLoggerADP::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SimpleLoggerADP *_t = static_cast<SimpleLoggerADP *>(_o);
        switch (_id) {
        case 0: _t->formattedMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< SimpleLoggerRoutingInfo::MessageCategory(*)>(_a[2]))); break;
        case 1: _t->receiveMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QDateTime(*)>(_a[3])),(*reinterpret_cast< SimpleLoggerRoutingInfo::MessageCategory(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SimpleLoggerADP::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SimpleLoggerADP::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SimpleLoggerADP,
      qt_meta_data_SimpleLoggerADP, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SimpleLoggerADP::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SimpleLoggerADP::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SimpleLoggerADP::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SimpleLoggerADP))
        return static_cast<void*>(const_cast< SimpleLoggerADP*>(this));
    return QObject::qt_metacast(_clname);
}

int SimpleLoggerADP::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void SimpleLoggerADP::formattedMessage(const QString & _t1, SimpleLoggerRoutingInfo::MessageCategory _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
