/****************************************************************************
** Meta object code from reading C++ file 'backupsetdialog.h'
**
** Created: Mon Apr 16 10:26:51 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "backupsetdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'backupsetdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BackupSetDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x0a,
      30,   16,   16,   16, 0x0a,
      45,   16,   16,   16, 0x0a,
      57,   16,   16,   16, 0x0a,
      68,   16,   16,   16, 0x0a,
      81,   16,   16,   16, 0x0a,
      96,   16,   16,   16, 0x0a,
     113,   16,   16,   16, 0x0a,
     127,   16,   16,   16, 0x0a,
     140,   16,   16,   16, 0x0a,
     155,   16,   16,   16, 0x0a,
     172,   16,   16,   16, 0x0a,
     194,  187,   16,   16, 0x0a,
     220,   16,   16,   16, 0x0a,
     236,   16,   16,   16, 0x0a,
     269,  252,   16,   16, 0x0a,
     318,  252,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_BackupSetDialog[] = {
    "BackupSetDialog\0\0copyFilter()\0"
    "insertFilter()\0delFilter()\0upFilter()\0"
    "downFilter()\0copyCriteria()\0"
    "insertCriteria()\0delCriteria()\0"
    "upCriteria()\0downCriteria()\0"
    "selectFromRoot()\0selectToRoot()\0method\0"
    "selectHashMethod(QString)\0saveBackupSet()\0"
    "loadBackupSet()\0current,previous\0"
    "currentFilterRowChanged(QModelIndex,QModelIndex)\0"
    "currentCriteriaRowChanged(QModelIndex,QModelIndex)\0"
};

void BackupSetDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BackupSetDialog *_t = static_cast<BackupSetDialog *>(_o);
        switch (_id) {
        case 0: _t->copyFilter(); break;
        case 1: _t->insertFilter(); break;
        case 2: _t->delFilter(); break;
        case 3: _t->upFilter(); break;
        case 4: _t->downFilter(); break;
        case 5: _t->copyCriteria(); break;
        case 6: _t->insertCriteria(); break;
        case 7: _t->delCriteria(); break;
        case 8: _t->upCriteria(); break;
        case 9: _t->downCriteria(); break;
        case 10: _t->selectFromRoot(); break;
        case 11: _t->selectToRoot(); break;
        case 12: _t->selectHashMethod((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 13: _t->saveBackupSet(); break;
        case 14: _t->loadBackupSet(); break;
        case 15: _t->currentFilterRowChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        case 16: _t->currentCriteriaRowChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData BackupSetDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject BackupSetDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_BackupSetDialog,
      qt_meta_data_BackupSetDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BackupSetDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BackupSetDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BackupSetDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BackupSetDialog))
        return static_cast<void*>(const_cast< BackupSetDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int BackupSetDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
