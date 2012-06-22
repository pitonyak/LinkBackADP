/****************************************************************************
** Meta object code from reading C++ file 'linkbackupadp.h'
**
** Created: Mon Apr 16 10:26:50 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "linkbackupadp.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'linkbackupadp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LinkBackupADP[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x0a,
      41,   14,   14,   14, 0x0a,
      73,   14,   14,   14, 0x0a,
     106,   14,   14,   14, 0x0a,
     166,  140,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_LinkBackupADP[] = {
    "LinkBackupADP\0\0on_actionExit_activated()\0"
    "on_actionEditBackup_activated()\0"
    "on_actionStartBackup_activated()\0"
    "on_actionCancelBackup_activated()\0"
    "formattedMessage,category\0"
    "formattedMessage(QString,SimpleLoggerRoutingInfo::MessageCategory)\0"
};

void LinkBackupADP::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LinkBackupADP *_t = static_cast<LinkBackupADP *>(_o);
        switch (_id) {
        case 0: _t->on_actionExit_activated(); break;
        case 1: _t->on_actionEditBackup_activated(); break;
        case 2: _t->on_actionStartBackup_activated(); break;
        case 3: _t->on_actionCancelBackup_activated(); break;
        case 4: _t->formattedMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< SimpleLoggerRoutingInfo::MessageCategory(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData LinkBackupADP::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LinkBackupADP::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_LinkBackupADP,
      qt_meta_data_LinkBackupADP, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LinkBackupADP::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LinkBackupADP::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LinkBackupADP::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LinkBackupADP))
        return static_cast<void*>(const_cast< LinkBackupADP*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int LinkBackupADP::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
