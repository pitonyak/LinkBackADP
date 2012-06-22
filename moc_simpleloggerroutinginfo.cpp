/****************************************************************************
** Meta object code from reading C++ file 'simpleloggerroutinginfo.h'
**
** Created: Mon Apr 16 10:26:57 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "simpleloggerroutinginfo.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'simpleloggerroutinginfo.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SimpleLoggerRoutingInfo[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       3,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
      24, 0x0,    5,   26,
      40, 0x0,    5,   36,
      57, 0x0,    3,   46,

 // enum data: key, value
      72, uint(SimpleLoggerRoutingInfo::TraceMessage),
      85, uint(SimpleLoggerRoutingInfo::DebugMessage),
      98, uint(SimpleLoggerRoutingInfo::InformationMessage),
     117, uint(SimpleLoggerRoutingInfo::WarningMessage),
     132, uint(SimpleLoggerRoutingInfo::ErrorMessage),
     145, uint(SimpleLoggerRoutingInfo::DateTimeComponent),
     163, uint(SimpleLoggerRoutingInfo::MessageTypeComponent),
     184, uint(SimpleLoggerRoutingInfo::MessageTextComponent),
     205, uint(SimpleLoggerRoutingInfo::MessageLocationComponent),
     230, uint(SimpleLoggerRoutingInfo::MessageText),
     242, uint(SimpleLoggerRoutingInfo::RouteFile),
     252, uint(SimpleLoggerRoutingInfo::RouteEmit),
     262, uint(SimpleLoggerRoutingInfo::RouteQDebug),

       0        // eod
};

static const char qt_meta_stringdata_SimpleLoggerRoutingInfo[] = {
    "SimpleLoggerRoutingInfo\0MessageCategory\0"
    "MessageComponent\0MessageRouting\0"
    "TraceMessage\0DebugMessage\0InformationMessage\0"
    "WarningMessage\0ErrorMessage\0"
    "DateTimeComponent\0MessageTypeComponent\0"
    "MessageTextComponent\0MessageLocationComponent\0"
    "MessageText\0RouteFile\0RouteEmit\0"
    "RouteQDebug\0"
};

void SimpleLoggerRoutingInfo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData SimpleLoggerRoutingInfo::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SimpleLoggerRoutingInfo::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SimpleLoggerRoutingInfo,
      qt_meta_data_SimpleLoggerRoutingInfo, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SimpleLoggerRoutingInfo::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SimpleLoggerRoutingInfo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SimpleLoggerRoutingInfo::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SimpleLoggerRoutingInfo))
        return static_cast<void*>(const_cast< SimpleLoggerRoutingInfo*>(this));
    return QObject::qt_metacast(_clname);
}

int SimpleLoggerRoutingInfo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
