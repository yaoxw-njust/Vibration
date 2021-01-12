/****************************************************************************
** Meta object code from reading C++ file 'main_logic_tcp.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../main_logic_tcp.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main_logic_tcp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainLogicTcp_t {
    QByteArrayData data[10];
    char stringdata0[112];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainLogicTcp_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainLogicTcp_t qt_meta_stringdata_MainLogicTcp = {
    {
QT_MOC_LITERAL(0, 0, 12), // "MainLogicTcp"
QT_MOC_LITERAL(1, 13, 16), // "mainLogicSendMsg"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 3), // "msg"
QT_MOC_LITERAL(4, 35, 7), // "msgFlag"
QT_MOC_LITERAL(5, 43, 17), // "sendVibrationData"
QT_MOC_LITERAL(6, 61, 12), // "sendAxleData"
QT_MOC_LITERAL(7, 74, 18), // "sendElectricalData"
QT_MOC_LITERAL(8, 93, 12), // "mainLogicRun"
QT_MOC_LITERAL(9, 106, 5) // "bytes"

    },
    "MainLogicTcp\0mainLogicSendMsg\0\0msg\0"
    "msgFlag\0sendVibrationData\0sendAxleData\0"
    "sendElectricalData\0mainLogicRun\0bytes"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainLogicTcp[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   39,    2, 0x06 /* Public */,
       5,    0,   44,    2, 0x06 /* Public */,
       6,    0,   45,    2, 0x06 /* Public */,
       7,    0,   46,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   47,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QByteArray,    9,

       0        // eod
};

void MainLogicTcp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainLogicTcp *_t = static_cast<MainLogicTcp *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->mainLogicSendMsg((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->sendVibrationData(); break;
        case 2: _t->sendAxleData(); break;
        case 3: _t->sendElectricalData(); break;
        case 4: _t->mainLogicRun((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MainLogicTcp::*_t)(QString , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainLogicTcp::mainLogicSendMsg)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MainLogicTcp::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainLogicTcp::sendVibrationData)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (MainLogicTcp::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainLogicTcp::sendAxleData)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (MainLogicTcp::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainLogicTcp::sendElectricalData)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject MainLogicTcp::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MainLogicTcp.data,
      qt_meta_data_MainLogicTcp,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainLogicTcp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainLogicTcp::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainLogicTcp.stringdata0))
        return static_cast<void*>(const_cast< MainLogicTcp*>(this));
    return QObject::qt_metacast(_clname);
}

int MainLogicTcp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void MainLogicTcp::mainLogicSendMsg(QString _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainLogicTcp::sendVibrationData()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void MainLogicTcp::sendAxleData()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void MainLogicTcp::sendElectricalData()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
