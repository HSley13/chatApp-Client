#pragma once

#include "chat_protocol.h"
#include <QtWidgets>
#include <QtCore>
#include <QtMultimedia>
#include <QWebSocket>
#include <QWebSocketServer>
#include <emscripten.h>
#include <sys/stat.h>
#include <cstdio>

class client_manager : public QMainWindow
{
    Q_OBJECT
public:
    client_manager(QWidget *parent = nullptr);

    void send_text(const QString &sender, const QString &receiver, const QString &text, const QString &time);
    void send_name(const QString &name);
    void send_is_typing(const QString &sender, const QString &receiver);

    void save_file(const QString &sender, const QString &file_name, const QByteArray &file_data, const QString &time);
    void save_audio(const QString &sender, const QString &audio_name, const QByteArray &audio_data, const QString &time);

    void send_audio(const QString &sender, const QString &receiver, const QString &audio_name, const QByteArray &audio_data, const QString &time);
    void send_lookup_friend(const QString &ID);

    void send_create_conversation(const int &conversation_ID, const QString &participant1, const int &participant1_ID, const QString &participant2, const int &participant2_ID);
    void send_save_conversation(const int &conversation_ID, const QString &sender, const QString &receiver, const QString &content, const QString &time);

    void send_save_data(const int &conversation_ID, const QString &sender, const QString &receiver, const QString &data_name, const QByteArray &data_data, const QString &type, const QString &time);

    void send_sign_up(const QString &phone_number, const QString &first_name, const QString &last_name, const QString &password, const QString &secret_question, const QString &secret_answer);
    void send_login_request(const QString &phone_number, const QString &password, const QString &time_zone);

    void send_file(const QString &sender, const QString &receiver, const QString &file_name, const QByteArray &file_data, const QString &time);

    void send_delete_message(const int &conversation_ID, const QString &sender, const QString &receiver, const QString &time);
    void send_delete_group_message(const int &group_ID, const QString &group_name, const QString &time);

    void send_create_new_group(const QString &adm, const QStringList &members, const QString &group_name);

    void mount_audio_IDBFS();
    void mount_file_IDBFS();

    void IDBFS_save_audio(const QString &audio_name, const QByteArray &audio_data, const int &size);
    QUrl get_audio_url(const QString &audio_name, const int &conversation_ID, const QString &type, const QString &time_UTC);
    void delete_audio_IDBFS(const QString &audio_name);

    void IDBFS_save_file(const QString &file_name, const QByteArray &file_data, const int &size);
    QUrl get_file_url(const QString &file_name, const int &conversation_ID, const QString &type, const QString &time_UTC);
    void delete_file_IDBFS(const QString &file_name);

    void send_group_is_typing(const int &group_ID, const QString &group_name, const QString &sender);
    void send_group_text(const int &group_ID, const QString &group_name, const QString &sender, const QString &message, const QString &time);
    void send_group_file(const int &group_ID, const QString &group_name, const QString &sender, const QString &file_name, const QByteArray &file_data, const QString &time);
    void send_group_audio(const int &group_ID, const QString &group_name, const QString &sender, const QString &audio_name, const QByteArray &audio_data, const QString &time);

    void save_group_file(const int &group_ID, const QString &group_name, const QString &sender, const QString &file_name, const QByteArray &file_data, const QString &time);
    void save_group_audio(const int &group_ID, const QString &group_name, const QString &sender, const QString &audio_name, const QByteArray &audio_data, const QString &time);

    void send_new_group_member_message(const int &group_ID, const QString &group_name, const QString &adm, const QString &group_member);
    void send_remove_group_member_message(const int &group_ID, const QString &group_name, const QString &adm, const QString &group_member);

    void send_delete_account_message(const QString &phone_number);

    void send_last_message_read(const int &conversation_ID, const QString &client_ID, const QString &time);
    void send_group_last_message_read(const int &group_ID, const QString &client_ID, const QString &time);

    const QString &my_ID() const;
    const QString &my_name() const;
    const QString &file_name() const;

    const void get_user_time() const;

    const QString &time_zone() const;

private:
    static QWebSocket *_socket;

    static chat_protocol *_protocol;

    static QWebSocketServer *_file_server;
    QWebSocket *_file_socket;

    static QString _my_ID;
    static QString _my_name;

    static QString _file_name;

    static QString _time_zone;

    QString _type;

signals:
    void text_message_received(const QString &sender, const QString &message, const QString &time);
    void is_typing_received(const QString &sender);

    void client_name_changed(const QString &old_name, const QString &client_name);
    void client_connected(const QString &client_name);
    void client_disconnected(const QString &client_name);

    void socket_disconnected();

    void audio_received(const QString &sender, const QString &audio, const QString &time);
    void file_received(const QString &sender, const QString &file_name, const QString &time);

    void client_added_you(const int &conversation_ID, const QString &name, const QString &ID);
    void lookup_friend_result(const int &conversation_ID, const QString &name, bool true_or_false);

    void login_request(const QString &hashed_password, bool true_or_false, const QHash<int, QHash<QString, QString>> &friend_list, const QStringList &online_friends, const QHash<int, QStringList> &messages, const QHash<int, QHash<int, QString>> &group_list, const QHash<int, QStringList> &group_messages, const QHash<int, QStringList> &groups_members);

    void delete_message(const QString &sender, const QString &time);

    void saving_file(const QString &message);

    void new_group_ID(const int &group_ID);

    void added_to_group(const int &group_ID, const QString &adm, const QStringList &group_members, const QString &group_name);

    void group_is_typing_received(const int &group_ID, const QString &group_name, const QString &sender);
    void group_text_received(const int &group_ID, const QString &group_name, const QString &sender, const QString &message, const QString &time);

    void group_audio_received(const int &group_ID, const QString &group_name, const QString &sender, const QString &audio, const QString &time);
    void group_file_received(const int &group_ID, const QString &group_name, const QString &sender, const QString &file_name, const QString &time);

    void removed_from_group(const int &group_ID, const QString &group_name, const QString &adm, const QString &removed_member);

private slots:
    void on_disconnected();

    void on_binary_message_received(const QByteArray &message);
};