/****************************************************************************
** Meta object code from reading C++ file 'bp-nfoview.h'
**
** Created: Tue May 24 20:58:01 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "bp-nfoview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bp-nfoview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Ui__Ui_MainWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x0a,
      34,   18,   18,   18, 0x0a,
      47,   18,   18,   18, 0x0a,
      61,   18,   18,   18, 0x0a,
      75,   18,   18,   18, 0x0a,
      96,   18,   18,   18, 0x0a,
     109,   18,   18,   18, 0x0a,
     129,   18,   18,   18, 0x0a,
     147,   18,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Ui__Ui_MainWindow[] = {
    "Ui::Ui_MainWindow\0\0SBaction(bool)\0"
    "OpenAction()\0AboutAction()\0ColorAction()\0"
    "DefaultColorAction()\0FontAction()\0"
    "DefaultFontAction()\0loadFile(QString)\0"
    "saveImageAction()\0"
};

const QMetaObject Ui::Ui_MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Ui__Ui_MainWindow,
      qt_meta_data_Ui__Ui_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Ui::Ui_MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Ui::Ui_MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Ui::Ui_MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Ui__Ui_MainWindow))
        return static_cast<void*>(const_cast< Ui_MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Ui::Ui_MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: SBaction((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: OpenAction(); break;
        case 2: AboutAction(); break;
        case 3: ColorAction(); break;
        case 4: DefaultColorAction(); break;
        case 5: FontAction(); break;
        case 6: DefaultFontAction(); break;
        case 7: loadFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: saveImageAction(); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
