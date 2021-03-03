/****************************************************************************
** Meta object code from reading C++ file 'vais_main_form.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../vais_main_form.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vais_main_form.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_VAISMainForm_t {
    QByteArrayData data[16];
    char stringdata0[227];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VAISMainForm_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VAISMainForm_t qt_meta_stringdata_VAISMainForm = {
    {
QT_MOC_LITERAL(0, 0, 12), // "VAISMainForm"
QT_MOC_LITERAL(1, 13, 17), // "sendMainFormClose"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 11), // "onActivated"
QT_MOC_LITERAL(4, 44, 33), // "QSystemTrayIcon::ActivationRe..."
QT_MOC_LITERAL(5, 78, 6), // "reason"
QT_MOC_LITERAL(6, 85, 9), // "__quitApp"
QT_MOC_LITERAL(7, 95, 19), // "__useNotepadShowLog"
QT_MOC_LITERAL(8, 115, 5), // "Index"
QT_MOC_LITERAL(9, 121, 11), // "__reportMsg"
QT_MOC_LITERAL(10, 133, 3), // "msg"
QT_MOC_LITERAL(11, 137, 8), // "alamFlag"
QT_MOC_LITERAL(12, 146, 19), // "__updataVibrateData"
QT_MOC_LITERAL(13, 166, 16), // "__updataAxleData"
QT_MOC_LITERAL(14, 183, 22), // "__updataElectricalData"
QT_MOC_LITERAL(15, 206, 20) // "__simulateBtnClicked"

    },
    "VAISMainForm\0sendMainFormClose\0\0"
    "onActivated\0QSystemTrayIcon::ActivationReason\0"
    "reason\0__quitApp\0__useNotepadShowLog\0"
    "Index\0__reportMsg\0msg\0alamFlag\0"
    "__updataVibrateData\0__updataAxleData\0"
    "__updataElectricalData\0__simulateBtnClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VAISMainForm[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   60,    2, 0x08 /* Private */,
       6,    0,   63,    2, 0x08 /* Private */,
       7,    1,   64,    2, 0x08 /* Private */,
       9,    2,   67,    2, 0x08 /* Private */,
      12,    0,   72,    2, 0x08 /* Private */,
      13,    0,   73,    2, 0x08 /* Private */,
      14,    0,   74,    2, 0x08 /* Private */,
      15,    0,   75,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QModelIndex,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   10,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void VAISMainForm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VAISMainForm *_t = static_cast<VAISMainForm *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendMainFormClose(); break;
        case 1: _t->onActivated((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*)>(_a[1]))); break;
        case 2: _t->__quitApp(); break;
        case 3: _t->__useNotepadShowLog((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 4: _t->__reportMsg((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->__updataVibrateData(); break;
        case 6: _t->__updataAxleData(); break;
        case 7: _t->__updataElectricalData(); break;
        case 8: _t->__simulateBtnClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (VAISMainForm::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VAISMainForm::sendMainFormClose)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject VAISMainForm::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_VAISMainForm.data,
      qt_meta_data_VAISMainForm,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *VAISMainForm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VAISMainForm::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_VAISMainForm.stringdata0))
        return static_cast<void*>(const_cast< VAISMainForm*>(this));
    return QWidget::qt_metacast(_clname);
}

int VAISMainForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void VAISMainForm::sendMainFormClose()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
