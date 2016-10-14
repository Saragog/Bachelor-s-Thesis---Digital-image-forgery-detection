/****************************************************************************
** Meta object code from reading C++ file 'WindowMenu.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "WindowMenu.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WindowMenu.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WindowMenu[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      25,   11,   11,   11, 0x0a,
      45,   11,   11,   11, 0x0a,
      68,   11,   11,   11, 0x0a,
      91,   11,   11,   11, 0x0a,
     118,  113,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_WindowMenu[] = {
    "WindowMenu\0\0chooseFile()\0setMapperExivInfo()\0"
    "setMapperSecureImage()\0setMapperSecureCheck()\0"
    "setMapperTextEdited()\0path\0"
    "validateImage(QString)\0"
};

void WindowMenu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        WindowMenu *_t = static_cast<WindowMenu *>(_o);
        switch (_id) {
        case 0: _t->chooseFile(); break;
        case 1: _t->setMapperExivInfo(); break;
        case 2: _t->setMapperSecureImage(); break;
        case 3: _t->setMapperSecureCheck(); break;
        case 4: _t->setMapperTextEdited(); break;
        case 5: _t->validateImage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData WindowMenu::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject WindowMenu::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_WindowMenu,
      qt_meta_data_WindowMenu, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &WindowMenu::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *WindowMenu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *WindowMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WindowMenu))
        return static_cast<void*>(const_cast< WindowMenu*>(this));
    return QWidget::qt_metacast(_clname);
}

int WindowMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
