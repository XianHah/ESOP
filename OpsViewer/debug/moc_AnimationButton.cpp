/****************************************************************************
** Meta object code from reading C++ file 'AnimationButton.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../AnimationButton.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AnimationButton.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AnimationButton_t {
    QByteArrayData data[9];
    char stringdata0[87];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AnimationButton_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AnimationButton_t qt_meta_stringdata_AnimationButton = {
    {
QT_MOC_LITERAL(0, 0, 15), // "AnimationButton"
QT_MOC_LITERAL(1, 16, 17), // "enterImageChanged"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 5), // "index"
QT_MOC_LITERAL(4, 41, 17), // "leaveImageChanged"
QT_MOC_LITERAL(5, 59, 7), // "setText"
QT_MOC_LITERAL(6, 67, 4), // "text"
QT_MOC_LITERAL(7, 72, 8), // "setImage"
QT_MOC_LITERAL(8, 81, 5) // "image"

    },
    "AnimationButton\0enterImageChanged\0\0"
    "index\0leaveImageChanged\0setText\0text\0"
    "setImage\0image"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AnimationButton[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x08 /* Private */,
       4,    1,   37,    2, 0x08 /* Private */,
       5,    1,   40,    2, 0x0a /* Public */,
       7,    1,   43,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QVariant,    3,
    QMetaType::Void, QMetaType::QVariant,    3,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    8,

       0        // eod
};

void AnimationButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AnimationButton *_t = static_cast<AnimationButton *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->enterImageChanged((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 1: _t->leaveImageChanged((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 2: _t->setText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->setImage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject AnimationButton::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_AnimationButton.data,
      qt_meta_data_AnimationButton,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *AnimationButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AnimationButton::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AnimationButton.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int AnimationButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
