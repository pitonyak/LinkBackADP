/****************************************************************************
** Meta object code from reading C++ file 'linkbackfilter.h'
**
** Created: Mon Apr 16 10:26:52 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "linkbackfilter.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'linkbackfilter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LinkBackFilter[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       8,   59, // properties
       2,   91, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      48,   15,   15,   15, 0x05,
      82,   15,   15,   15, 0x05,
     111,   15,   15,   15, 0x05,
     143,   15,   15,   15, 0x05,
     174,   15,   15,   15, 0x05,
     199,   15,   15,   15, 0x05,
     223,   15,   15,   15, 0x05,
     246,   15,   15,   15, 0x05,

 // properties: name, type, flags
     283,  271, 0x0049510f,
     308,  295, 0x0049510f,
     341,  321, 0x0049510f,
     362,  357, 0x01495107,
     381,  357, 0x01495107,
     399,  357, 0x01495107,
     411,  357, 0x01495107,
     431,  422, 0xff495107,

 // properties: notify_signal_id
       0,
       1,
       2,
       3,
       4,
       5,
       6,
       7,

 // enums: name, flags, count, data
     271, 0x0,    9,   99,
     295, 0x0,    7,  117,

 // enum data: key, value
     437, uint(LinkBackFilter::Less),
     442, uint(LinkBackFilter::LessEqual),
     452, uint(LinkBackFilter::Equal),
     458, uint(LinkBackFilter::GreaterEqual),
     471, uint(LinkBackFilter::Greater),
     479, uint(LinkBackFilter::NotEqual),
     488, uint(LinkBackFilter::RegularExpression),
     506, uint(LinkBackFilter::FileSpec),
     515, uint(LinkBackFilter::Contains),
     524, uint(LinkBackFilter::Date),
     529, uint(LinkBackFilter::DateTime),
     538, uint(LinkBackFilter::FullPath),
     547, uint(LinkBackFilter::Name),
     552, uint(LinkBackFilter::Size),
     557, uint(LinkBackFilter::Time),
     562, uint(LinkBackFilter::PathOnly),

       0        // eod
};

static const char qt_meta_stringdata_LinkBackFilter[] = {
    "LinkBackFilter\0\0compareTypeChanged(CompareType)\0"
    "compareFieldChanged(CompareField)\0"
    "caseSensitivityChanged(bool)\0"
    "invertFilterResultChanged(bool)\0"
    "filterMeansAcceptChanged(bool)\0"
    "filterFilesChanged(bool)\0"
    "filterDirsChanged(bool)\0valueChanged(QVariant)\0"
    "multiValuedChanged(bool)\0CompareType\0"
    "compareType\0CompareField\0compareField\0"
    "Qt::CaseSensitivity\0caseSensitivity\0"
    "bool\0invertFilterResult\0filterMeansAccept\0"
    "filterFiles\0filterDirs\0QVariant\0value\0"
    "Less\0LessEqual\0Equal\0GreaterEqual\0"
    "Greater\0NotEqual\0RegularExpression\0"
    "FileSpec\0Contains\0Date\0DateTime\0"
    "FullPath\0Name\0Size\0Time\0PathOnly\0"
};

void LinkBackFilter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LinkBackFilter *_t = static_cast<LinkBackFilter *>(_o);
        switch (_id) {
        case 0: _t->compareTypeChanged((*reinterpret_cast< CompareType(*)>(_a[1]))); break;
        case 1: _t->compareFieldChanged((*reinterpret_cast< CompareField(*)>(_a[1]))); break;
        case 2: _t->caseSensitivityChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->invertFilterResultChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->filterMeansAcceptChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->filterFilesChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->filterDirsChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->valueChanged((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 8: _t->multiValuedChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData LinkBackFilter::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LinkBackFilter::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_LinkBackFilter,
      qt_meta_data_LinkBackFilter, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LinkBackFilter::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LinkBackFilter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LinkBackFilter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LinkBackFilter))
        return static_cast<void*>(const_cast< LinkBackFilter*>(this));
    return QObject::qt_metacast(_clname);
}

int LinkBackFilter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< CompareType*>(_v) = getCompareType(); break;
        case 1: *reinterpret_cast< CompareField*>(_v) = getCompareField(); break;
        case 2: *reinterpret_cast< Qt::CaseSensitivity*>(_v) = getCaseSensitivity(); break;
        case 3: *reinterpret_cast< bool*>(_v) = isInvertFilterResult(); break;
        case 4: *reinterpret_cast< bool*>(_v) = isFilterMeansAccept(); break;
        case 5: *reinterpret_cast< bool*>(_v) = isFilterFiles(); break;
        case 6: *reinterpret_cast< bool*>(_v) = isFilterDirs(); break;
        case 7: *reinterpret_cast< QVariant*>(_v) = getValue(); break;
        }
        _id -= 8;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setCompareType(*reinterpret_cast< CompareType*>(_v)); break;
        case 1: setCompareField(*reinterpret_cast< CompareField*>(_v)); break;
        case 2: setCaseSensitivity(*reinterpret_cast< Qt::CaseSensitivity*>(_v)); break;
        case 3: setInvertFilterResult(*reinterpret_cast< bool*>(_v)); break;
        case 4: setFilterMeansAccept(*reinterpret_cast< bool*>(_v)); break;
        case 5: setFilterFiles(*reinterpret_cast< bool*>(_v)); break;
        case 6: setFilterDirs(*reinterpret_cast< bool*>(_v)); break;
        case 7: setValue(*reinterpret_cast< QVariant*>(_v)); break;
        }
        _id -= 8;
    } else if (_c == QMetaObject::ResetProperty) {
        switch (_id) {
        case 0: setCompareTypeDefault(); break;
        case 1: setCompareFieldDefault(); break;
        case 2: setCaseSensitivityDefault(); break;
        case 3: setInvertFilterResultDefault(); break;
        case 4: setFilterMeansAcceptDefault(); break;
        case 5: setFilterFilesDefault(); break;
        case 6: setFilterDirsDefault(); break;
        case 7: setValueDefault(); break;
        }
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 8;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void LinkBackFilter::compareTypeChanged(CompareType _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void LinkBackFilter::compareFieldChanged(CompareField _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void LinkBackFilter::caseSensitivityChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void LinkBackFilter::invertFilterResultChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void LinkBackFilter::filterMeansAcceptChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void LinkBackFilter::filterFilesChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void LinkBackFilter::filterDirsChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void LinkBackFilter::valueChanged(QVariant _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void LinkBackFilter::multiValuedChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_END_MOC_NAMESPACE
