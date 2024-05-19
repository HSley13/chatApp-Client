/****************************************************************************
** Meta object code from reading C++ file 'client_chat_window.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../client_chat_window.h"
#include <QtGui/qtextcursor.h>
#include <QtGui/qscreen.h>
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'client_chat_window.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSclient_chat_windowENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSclient_chat_windowENDCLASS = QtMocHelpers::stringData(
    "client_chat_window",
    "swipe_right",
    "",
    "client_name_changed",
    "old_name",
    "client_name",
    "client_connected",
    "client_disconnected",
    "text_message_received",
    "sender",
    "message",
    "is_typing_received",
    "socket_disconnected",
    "update_button_file",
    "data_received_sent",
    "client_added_you",
    "conversation_ID",
    "name",
    "ID",
    "lookup_friend_result",
    "audio_received",
    "path",
    "file_saved",
    "item_deleted",
    "QListWidgetItem*",
    "item",
    "login_request",
    "hashed_password",
    "true_or_false",
    "QHash<int,QHash<QString,int>>",
    "friend_list",
    "online_friends",
    "QHash<int,QList<QString>>",
    "messages",
    "QHash<int,QHash<QString,QByteArray>>",
    "binary_data",
    "send_message",
    "send_file",
    "send_file_client",
    "folder",
    "on_file_saved",
    "on_init_receiving_file",
    "file_name",
    "file_size",
    "on_init_receiving_file_client",
    "start_recording",
    "on_duration_changed",
    "duration",
    "play_audio",
    "source",
    "QPushButton*",
    "audio",
    "QSlider*",
    "slider",
    "on_item_deleted"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSclient_chat_windowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      26,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      15,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  170,    2, 0x06,    1 /* Public */,
       3,    2,  171,    2, 0x06,    2 /* Public */,
       6,    1,  176,    2, 0x06,    5 /* Public */,
       7,    1,  179,    2, 0x06,    7 /* Public */,
       8,    2,  182,    2, 0x06,    9 /* Public */,
      11,    1,  187,    2, 0x06,   12 /* Public */,
      12,    0,  190,    2, 0x06,   14 /* Public */,
      13,    0,  191,    2, 0x06,   15 /* Public */,
      14,    1,  192,    2, 0x06,   16 /* Public */,
      15,    3,  195,    2, 0x06,   18 /* Public */,
      19,    2,  202,    2, 0x06,   22 /* Public */,
      20,    2,  207,    2, 0x06,   25 /* Public */,
      22,    2,  212,    2, 0x06,   28 /* Public */,
      23,    1,  217,    2, 0x06,   31 /* Public */,
      26,    6,  220,    2, 0x06,   33 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      36,    0,  233,    2, 0x08,   40 /* Private */,
      37,    0,  234,    2, 0x08,   41 /* Private */,
      38,    0,  235,    2, 0x08,   42 /* Private */,
      39,    0,  236,    2, 0x08,   43 /* Private */,
      40,    1,  237,    2, 0x08,   44 /* Private */,
      41,    2,  240,    2, 0x08,   46 /* Private */,
      44,    4,  245,    2, 0x08,   49 /* Private */,
      45,    0,  254,    2, 0x08,   54 /* Private */,
      46,    1,  255,    2, 0x08,   55 /* Private */,
      48,    3,  258,    2, 0x08,   57 /* Private */,
      54,    1,  265,    2, 0x08,   61 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    4,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    9,   10,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::QString,   16,   17,   18,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   16,   17,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    9,   21,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    9,   21,
    QMetaType::Void, 0x80000000 | 24,   25,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool, 0x80000000 | 29, QMetaType::QStringList, 0x80000000 | 32, 0x80000000 | 34,   27,   28,   30,   31,   33,   35,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   21,
    QMetaType::Void, QMetaType::QString, QMetaType::LongLong,   42,   43,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::LongLong,    9,   18,   42,   43,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,   47,
    QMetaType::Void, QMetaType::QUrl, 0x80000000 | 50, 0x80000000 | 52,   49,   51,   53,
    QMetaType::Void, 0x80000000 | 24,   25,

       0        // eod
};

Q_CONSTINIT const QMetaObject client_chat_window::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSclient_chat_windowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSclient_chat_windowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSclient_chat_windowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<client_chat_window, std::true_type>,
        // method 'swipe_right'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'client_name_changed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'client_connected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'client_disconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'text_message_received'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'is_typing_received'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'socket_disconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'update_button_file'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'data_received_sent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'client_added_you'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'lookup_friend_result'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'audio_received'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'file_saved'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'item_deleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>,
        // method 'login_request'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<QHash<int,QHash<QString,int>>, std::false_type>,
        QtPrivate::TypeAndForceComplete<QList<QString>, std::false_type>,
        QtPrivate::TypeAndForceComplete<QHash<int,QVector<QString>>, std::false_type>,
        QtPrivate::TypeAndForceComplete<QHash<int,QHash<QString,QByteArray>>, std::false_type>,
        // method 'send_message'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'send_file'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'send_file_client'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'folder'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_file_saved'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'on_init_receiving_file'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        // method 'on_init_receiving_file_client'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        // method 'start_recording'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_duration_changed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        // method 'play_audio'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QUrl &, std::false_type>,
        QtPrivate::TypeAndForceComplete<QPushButton *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QSlider *, std::false_type>,
        // method 'on_item_deleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>
    >,
    nullptr
} };

void client_chat_window::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<client_chat_window *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->swipe_right(); break;
        case 1: _t->client_name_changed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 2: _t->client_connected((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->client_disconnected((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->text_message_received((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 5: _t->is_typing_received((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->socket_disconnected(); break;
        case 7: _t->update_button_file(); break;
        case 8: _t->data_received_sent((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 9: _t->client_added_you((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 10: _t->lookup_friend_result((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 11: _t->audio_received((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 12: _t->file_saved((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 13: _t->item_deleted((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 14: _t->login_request((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QHash<int,QHash<QString,int>>>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QList<QString>>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QHash<int,QList<QString>>>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<QHash<int,QHash<QString,QByteArray>>>>(_a[6]))); break;
        case 15: _t->send_message(); break;
        case 16: _t->send_file(); break;
        case 17: _t->send_file_client(); break;
        case 18: _t->folder(); break;
        case 19: _t->on_file_saved((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 20: _t->on_init_receiving_file((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<qint64>>(_a[2]))); break;
        case 21: _t->on_init_receiving_file_client((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<qint64>>(_a[4]))); break;
        case 22: _t->start_recording(); break;
        case 23: _t->on_duration_changed((*reinterpret_cast< std::add_pointer_t<qint64>>(_a[1]))); break;
        case 24: _t->play_audio((*reinterpret_cast< std::add_pointer_t<QUrl>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QPushButton*>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QSlider*>>(_a[3]))); break;
        case 25: _t->on_item_deleted((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 24:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QPushButton* >(); break;
            case 2:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QSlider* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (client_chat_window::*)();
            if (_t _q_method = &client_chat_window::swipe_right; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (client_chat_window::*)(QString , QString );
            if (_t _q_method = &client_chat_window::client_name_changed; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (client_chat_window::*)(QString );
            if (_t _q_method = &client_chat_window::client_connected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (client_chat_window::*)(QString );
            if (_t _q_method = &client_chat_window::client_disconnected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (client_chat_window::*)(QString , QString );
            if (_t _q_method = &client_chat_window::text_message_received; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (client_chat_window::*)(QString );
            if (_t _q_method = &client_chat_window::is_typing_received; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (client_chat_window::*)();
            if (_t _q_method = &client_chat_window::socket_disconnected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (client_chat_window::*)();
            if (_t _q_method = &client_chat_window::update_button_file; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (client_chat_window::*)(QString );
            if (_t _q_method = &client_chat_window::data_received_sent; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (client_chat_window::*)(int , QString , QString );
            if (_t _q_method = &client_chat_window::client_added_you; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (client_chat_window::*)(int , QString );
            if (_t _q_method = &client_chat_window::lookup_friend_result; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (client_chat_window::*)(QString , QString );
            if (_t _q_method = &client_chat_window::audio_received; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (client_chat_window::*)(QString , QString );
            if (_t _q_method = &client_chat_window::file_saved; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (client_chat_window::*)(QListWidgetItem * );
            if (_t _q_method = &client_chat_window::item_deleted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (client_chat_window::*)(QString , bool , QHash<int,QHash<QString,int>> , QList<QString> , QHash<int,QVector<QString>> , QHash<int,QHash<QString,QByteArray>> );
            if (_t _q_method = &client_chat_window::login_request; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 14;
                return;
            }
        }
    }
}

const QMetaObject *client_chat_window::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *client_chat_window::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSclient_chat_windowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int client_chat_window::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 26)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 26)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    }
    return _id;
}

// SIGNAL 0
void client_chat_window::swipe_right()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void client_chat_window::client_name_changed(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void client_chat_window::client_connected(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void client_chat_window::client_disconnected(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void client_chat_window::text_message_received(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void client_chat_window::is_typing_received(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void client_chat_window::socket_disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void client_chat_window::update_button_file()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void client_chat_window::data_received_sent(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void client_chat_window::client_added_you(int _t1, QString _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void client_chat_window::lookup_friend_result(int _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void client_chat_window::audio_received(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void client_chat_window::file_saved(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void client_chat_window::item_deleted(QListWidgetItem * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void client_chat_window::login_request(QString _t1, bool _t2, QHash<int,QHash<QString,int>> _t3, QList<QString> _t4, QHash<int,QVector<QString>> _t5, QHash<int,QHash<QString,QByteArray>> _t6)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t6))) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}
QT_WARNING_POP
