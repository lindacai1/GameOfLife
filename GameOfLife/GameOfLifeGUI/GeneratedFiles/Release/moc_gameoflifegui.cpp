/****************************************************************************
** Meta object code from reading C++ file 'gameoflifegui.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../gameoflifegui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gameoflifegui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GameOfLifeGUI_t {
    QByteArrayData data[9];
    char stringdata0[135];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GameOfLifeGUI_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GameOfLifeGUI_t qt_meta_stringdata_GameOfLifeGUI = {
    {
QT_MOC_LITERAL(0, 0, 13), // "GameOfLifeGUI"
QT_MOC_LITERAL(1, 14, 22), // "on_startButton_clicked"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 1), // "x"
QT_MOC_LITERAL(4, 40, 21), // "on_stopButton_clicked"
QT_MOC_LITERAL(5, 62, 16), // "on_label_clicked"
QT_MOC_LITERAL(6, 79, 21), // "on_nextButton_clicked"
QT_MOC_LITERAL(7, 101, 26), // "on_randBoardButton_clicked"
QT_MOC_LITERAL(8, 128, 6) // "update"

    },
    "GameOfLifeGUI\0on_startButton_clicked\0"
    "\0x\0on_stopButton_clicked\0on_label_clicked\0"
    "on_nextButton_clicked\0on_randBoardButton_clicked\0"
    "update"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GameOfLifeGUI[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x08 /* Private */,
       4,    1,   47,    2, 0x08 /* Private */,
       5,    0,   50,    2, 0x08 /* Private */,
       6,    1,   51,    2, 0x08 /* Private */,
       7,    1,   54,    2, 0x08 /* Private */,
       8,    0,   57,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,

       0        // eod
};

void GameOfLifeGUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GameOfLifeGUI *_t = static_cast<GameOfLifeGUI *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_startButton_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->on_stopButton_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->on_label_clicked(); break;
        case 3: _t->on_nextButton_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->on_randBoardButton_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->update(); break;
        default: ;
        }
    }
}

const QMetaObject GameOfLifeGUI::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_GameOfLifeGUI.data,
      qt_meta_data_GameOfLifeGUI,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *GameOfLifeGUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GameOfLifeGUI::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_GameOfLifeGUI.stringdata0))
        return static_cast<void*>(const_cast< GameOfLifeGUI*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int GameOfLifeGUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
