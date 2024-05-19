/****************************************************************************
** Meta object code from reading C++ file 'client_main_window.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../client_main_window.h"
#include <QtGui/qtextcursor.h>
#include <QtGui/qscreen.h>
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'client_main_window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.0. It"
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

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSclient_main_windowENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSclient_main_windowENDCLASS = QtMocHelpers::stringData(
    "client_main_window",
    "swipe_right",
    "",
    "on_swipe_right",
    "on_sign_in",
    "on_login_request",
    "hashed_password",
    "true_or_false",
    "QHash<int,QHash<QString,int>>",
    "friend_list",
    "online_friends",
    "QHash<int,QList<QString>>",
    "messages",
    "QHash<int,QHash<QString,QByteArray>>",
    "binary_data",
    "on_client_name_changed",
    "old_name",
    "client_name",
    "on_client_disconnected",
    "on_client_connected",
    "on_text_message_received",
    "sender",
    "message",
    "on_name_changed",
    "on_item_clicked",
    "QListWidgetItem*",
    "item",
    "new_conversation",
    "name",
    "on_client_added_you",
    "conversation_ID",
    "ID",
    "on_lookup_friend_result",
    "full_name",
    "on_audio_received",
    "path",
    "on_file_saved",
    "on_item_deleted"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSclient_main_windowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  110,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       3,    0,  111,    2, 0x08,    2 /* Private */,
       4,    0,  112,    2, 0x08,    3 /* Private */,
       5,    6,  113,    2, 0x08,    4 /* Private */,
      15,    2,  126,    2, 0x08,   11 /* Private */,
      18,    1,  131,    2, 0x08,   14 /* Private */,
      19,    1,  134,    2, 0x08,   16 /* Private */,
      20,    2,  137,    2, 0x08,   18 /* Private */,
      23,    0,  142,    2, 0x08,   21 /* Private */,
      24,    1,  143,    2, 0x08,   22 /* Private */,
      27,    1,  146,    2, 0x08,   24 /* Private */,
      29,    3,  149,    2, 0x08,   26 /* Private */,
      32,    2,  156,    2, 0x08,   30 /* Private */,
      34,    2,  161,    2, 0x08,   33 /* Private */,
      36,    2,  166,    2, 0x08,   36 /* Private */,
      37,    1,  171,    2, 0x08,   39 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool, 0x80000000 | 8, QMetaType::QStringList, 0x80000000 | 11, 0x80000000 | 13,    6,    7,    9,   10,   12,   14,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   16,   17,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   21,   22,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 25,   26,
    QMetaType::Void, QMetaType::QString,   28,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::QString,   30,   28,   31,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   30,   33,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   21,   35,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   21,   35,
    QMetaType::Void, 0x80000000 | 25,   26,

       0        // eod
};

Q_CONSTINIT const QMetaObject client_main_window::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSclient_main_windowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSclient_main_windowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSclient_main_windowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<client_main_window, std::true_type>,
        // method 'swipe_right'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_swipe_right'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_sign_in'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_login_request'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<QHash<int,QHash<QString,int>>, std::false_type>,
        QtPrivate::TypeAndForceComplete<QList<QString>, std::false_type>,
        QtPrivate::TypeAndForceComplete<QHash<int,QVector<QString>>, std::false_type>,
        QtPrivate::TypeAndForceComplete<QHash<int,QHash<QString,QByteArray>>, std::false_type>,
        // method 'on_client_name_changed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'on_client_disconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'on_client_connected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'on_text_message_received'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'on_name_changed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_item_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>,
        // method 'new_conversation'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_client_added_you'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'on_lookup_friend_result'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'on_audio_received'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'on_file_saved'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'on_item_deleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>
    >,
    nullptr
} };

void client_main_window::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<client_main_window *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->swipe_right(); break;
        case 1: _t->on_swipe_right(); break;
        case 2: _t->on_sign_in(); break;
        case 3: _t->on_login_request((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QHash<int,QHash<QString,int>>>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QList<QString>>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QHash<int,QList<QString>>>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<QHash<int,QHash<QString,QByteArray>>>>(_a[6]))); break;
        case 4: _t->on_client_name_changed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 5: _t->on_client_disconnected((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->on_client_connected((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->on_text_message_received((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 8: _t->on_name_changed(); break;
        case 9: _t->on_item_clicked((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 10: _t->new_conversation((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 11: _t->on_client_added_you((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 12: _t->on_lookup_friend_result((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 13: _t->on_audio_received((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 14: _t->on_file_saved((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 15: _t->on_item_deleted((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (client_main_window::*)();
            if (_t _q_method = &client_main_window::swipe_right; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *client_main_window::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *client_main_window::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSclient_main_windowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int client_main_window::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void client_main_window::swipe_right()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
