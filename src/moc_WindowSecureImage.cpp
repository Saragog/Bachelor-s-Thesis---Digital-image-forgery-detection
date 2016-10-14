/****************************************************************************
** Meta object code from reading C++ file 'WindowSecureImage.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "WindowSecureImage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WindowSecureImage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WindowSecureImage[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x08,
      39,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_WindowSecureImage[] = {
    "WindowSecureImage\0\0chooseWhereToSave()\0"
    "setMapping()\0"
};

void WindowSecureImage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        WindowSecureImage *_t = static_cast<WindowSecureImage *>(_o);
        switch (_id) {
        case 0: _t->chooseWhereToSave(); break;
        case 1: _t->setMapping(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData WindowSecureImage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject WindowSecureImage::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_WindowSecureImage,
      qt_meta_data_WindowSecureImage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &WindowSecureImage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *WindowSecureImage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *WindowSecureImage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WindowSecureImage))
        return static_cast<void*>(const_cast< WindowSecureImage*>(this));
    return QWidget::qt_metacast(_clname);
}

int WindowSecureImage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
