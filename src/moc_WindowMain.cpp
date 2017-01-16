/****************************************************************************
** Meta object code from reading C++ file 'WindowMain.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "WindowMain.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WindowMain.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WindowMain[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      32,   23,   11,   11, 0x0a,
      54,   23,   11,   11, 0x0a,
      79,   23,   11,   11, 0x0a,
     104,   11,   11,   11, 0x0a,
     122,   11,   11,   11, 0x0a,
     152,  142,   11,   11, 0x0a,
     180,  175,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_WindowMain[] = {
    "WindowMain\0\0showMenu()\0fileName\0"
    "showExivData(QString)\0showSecureImage(QString)\0"
    "showSecureCheck(QString)\0showAllExivData()\0"
    "showExivDataAgain()\0newAccDif\0"
    "adjustToNewAccDif(int)\0path\0"
    "saveImage(QString)\0"
};

void WindowMain::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        WindowMain *_t = static_cast<WindowMain *>(_o);
        switch (_id) {
        case 0: _t->showMenu(); break;
        case 1: _t->showExivData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->showSecureImage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->showSecureCheck((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->showAllExivData(); break;
        case 5: _t->showExivDataAgain(); break;
        case 6: _t->adjustToNewAccDif((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->saveImage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData WindowMain::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject WindowMain::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_WindowMain,
      qt_meta_data_WindowMain, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &WindowMain::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *WindowMain::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *WindowMain::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WindowMain))
        return static_cast<void*>(const_cast< WindowMain*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int WindowMain::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
