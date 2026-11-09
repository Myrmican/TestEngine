/****************************************************************************
** Meta object code from reading C++ file 'widgetwindowagent.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../qwindowkit-src/src/widgets/widgetwindowagent.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'widgetwindowagent.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN3QWK17WidgetWindowAgentE_t {};
} // unnamed namespace

template <> constexpr inline auto QWK::WidgetWindowAgent::qt_create_metaobjectdata<qt_meta_tag_ZN3QWK17WidgetWindowAgentE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QWK::WidgetWindowAgent",
        "titleBarChanged",
        "",
        "QWidget*",
        "w",
        "systemButtonChanged",
        "SystemButton",
        "button"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'titleBarChanged'
        QtMocHelpers::SignalData<void(QWidget *)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'systemButtonChanged'
        QtMocHelpers::SignalData<void(SystemButton, QWidget *)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 6, 7 }, { 0x80000000 | 3, 4 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<WidgetWindowAgent, qt_meta_tag_ZN3QWK17WidgetWindowAgentE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QWK::WidgetWindowAgent::staticMetaObject = { {
    QMetaObject::SuperData::link<WindowAgentBase::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN3QWK17WidgetWindowAgentE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN3QWK17WidgetWindowAgentE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN3QWK17WidgetWindowAgentE_t>.metaTypes,
    nullptr
} };

void QWK::WidgetWindowAgent::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<WidgetWindowAgent *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->titleBarChanged((*reinterpret_cast<std::add_pointer_t<QWidget*>>(_a[1]))); break;
        case 1: _t->systemButtonChanged((*reinterpret_cast<std::add_pointer_t<SystemButton>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QWidget*>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QWidget* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QWidget* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (WidgetWindowAgent::*)(QWidget * )>(_a, &WidgetWindowAgent::titleBarChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (WidgetWindowAgent::*)(SystemButton , QWidget * )>(_a, &WidgetWindowAgent::systemButtonChanged, 1))
            return;
    }
}

const QMetaObject *QWK::WidgetWindowAgent::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QWK::WidgetWindowAgent::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN3QWK17WidgetWindowAgentE_t>.strings))
        return static_cast<void*>(this);
    return WindowAgentBase::qt_metacast(_clname);
}

int QWK::WidgetWindowAgent::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = WindowAgentBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void QWK::WidgetWindowAgent::titleBarChanged(QWidget * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void QWK::WidgetWindowAgent::systemButtonChanged(SystemButton _t1, QWidget * _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}
QT_WARNING_POP
