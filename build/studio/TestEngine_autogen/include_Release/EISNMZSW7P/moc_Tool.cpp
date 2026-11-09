/****************************************************************************
** Meta object code from reading C++ file 'Tool.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../studio/include/editor/tools/Tool.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Tool.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN6Engine10StudioToolE_t {};
} // unnamed namespace

template <> constexpr inline auto Engine::StudioTool::qt_create_metaobjectdata<qt_meta_tag_ZN6Engine10StudioToolE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Engine::StudioTool"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<StudioTool, qt_meta_tag_ZN6Engine10StudioToolE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Engine::StudioTool::staticMetaObject = { {
    QMetaObject::SuperData::link<QToolButton::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6Engine10StudioToolE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6Engine10StudioToolE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN6Engine10StudioToolE_t>.metaTypes,
    nullptr
} };

void Engine::StudioTool::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<StudioTool *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *Engine::StudioTool::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Engine::StudioTool::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6Engine10StudioToolE_t>.strings))
        return static_cast<void*>(this);
    return QToolButton::qt_metacast(_clname);
}

int Engine::StudioTool::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QToolButton::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_tag_ZN6Engine15StudioToolGroupE_t {};
} // unnamed namespace

template <> constexpr inline auto Engine::StudioToolGroup::qt_create_metaobjectdata<qt_meta_tag_ZN6Engine15StudioToolGroupE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Engine::StudioToolGroup"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<StudioToolGroup, qt_meta_tag_ZN6Engine15StudioToolGroupE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Engine::StudioToolGroup::staticMetaObject = { {
    QMetaObject::SuperData::link<QActionGroup::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6Engine15StudioToolGroupE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6Engine15StudioToolGroupE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN6Engine15StudioToolGroupE_t>.metaTypes,
    nullptr
} };

void Engine::StudioToolGroup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<StudioToolGroup *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *Engine::StudioToolGroup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Engine::StudioToolGroup::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6Engine15StudioToolGroupE_t>.strings))
        return static_cast<void*>(this);
    return QActionGroup::qt_metacast(_clname);
}

int Engine::StudioToolGroup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QActionGroup::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
