/****************************************************************************
** Meta object code from reading C++ file 'algorithm_base.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../algorithm_base.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'algorithm_base.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AlgorithmBase_t {
    QByteArrayData data[8];
    char stringdata0[91];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AlgorithmBase_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AlgorithmBase_t qt_meta_stringdata_AlgorithmBase = {
    {
QT_MOC_LITERAL(0, 0, 13), // "AlgorithmBase"
QT_MOC_LITERAL(1, 14, 16), // "algorithmSendMsg"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 3), // "msg"
QT_MOC_LITERAL(4, 36, 4), // "flag"
QT_MOC_LITERAL(5, 41, 17), // "sendVibrationData"
QT_MOC_LITERAL(6, 59, 12), // "sendAxleData"
QT_MOC_LITERAL(7, 72, 18) // "sendElectricalData"

    },
    "AlgorithmBase\0algorithmSendMsg\0\0msg\0"
    "flag\0sendVibrationData\0sendAxleData\0"
    "sendElectricalData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AlgorithmBase[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   34,    2, 0x06 /* Public */,
       5,    0,   39,    2, 0x06 /* Public */,
       6,    0,   40,    2, 0x06 /* Public */,
       7,    0,   41,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void AlgorithmBase::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AlgorithmBase *_t = static_cast<AlgorithmBase *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->algorithmSendMsg((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->sendVibrationData(); break;
        case 2: _t->sendAxleData(); break;
        case 3: _t->sendElectricalData(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AlgorithmBase::*_t)(QString , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AlgorithmBase::algorithmSendMsg)) {
                *result = 0;
            }
        }
        {
            typedef void (AlgorithmBase::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AlgorithmBase::sendVibrationData)) {
                *result = 1;
            }
        }
        {
            typedef void (AlgorithmBase::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AlgorithmBase::sendAxleData)) {
                *result = 2;
            }
        }
        {
            typedef void (AlgorithmBase::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AlgorithmBase::sendElectricalData)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject AlgorithmBase::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AlgorithmBase.data,
      qt_meta_data_AlgorithmBase,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AlgorithmBase::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AlgorithmBase::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AlgorithmBase.stringdata0))
        return static_cast<void*>(const_cast< AlgorithmBase*>(this));
    return QObject::qt_metacast(_clname);
}

int AlgorithmBase::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void AlgorithmBase::algorithmSendMsg(QString _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AlgorithmBase::sendVibrationData()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void AlgorithmBase::sendAxleData()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void AlgorithmBase::sendElectricalData()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
