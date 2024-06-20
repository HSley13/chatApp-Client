#pragma once

#include <QtWidgets>
#include <QtCore>
#include <QtMultimedia>
#include <QWebSocket>

class chat_protocol : public QMainWindow
{
    Q_OBJECT
public:
    chat_protocol(QWidget *parent = nullptr);

    enum message_type
    {
        text,
        is_typing,
        set_name,

        file,

        client_new_name,
        client_disconnected,
        client_connected,

        added_you,
        lookup_friend,
        create_conversation,
        save_message,

        audio,
        save_data,

        sign_up,
        login_request,

        delete_message,

        new_group,
        added_to_group,

        group_is_typing,
        group_text,
        group_file,
        group_audio,

        new_group_member,
        remove_group_member,
        request_data,

        delete_account
    };

    QByteArray set_text_message(const QString &sender, const QString &receiver, const QString &message, const QString &time);
    QByteArray set_is_typing_message(const QString &sender, const QString &receiver);
    QByteArray set_name_message(const QString &my_name, const QString &name);

    QByteArray set_file_message(const QString &sender, const QString &receiver, const QString &file_name, const QByteArray &file_data, const QString &time);
    QByteArray set_audio_message(const QString &sender, const QString &receiver, const QString &audio_name, const QString &time);
    QByteArray set_save_audio_message(const int &conversation_ID, const QString &sender, const QString &receiver, const QString &file_name, const QString &type, const QString &time);
    QByteArray set_save_file_message(const int &conversation_ID, const QString &sender, const QString &receiver, const QString &file_name, const QByteArray &file_data, const QString &type, const QString &time);

    QByteArray set_lookup_friend_message(const QString &ID);
    QByteArray set_create_conversation_message(const int &conversation_ID, const QString &participant1, const int &participant1_ID, const QString &participant2, const int &participant2_ID);
    QByteArray set_save_message_message(const int &conversation_ID, const QString &sender, const QString &receiver, const QString &content, const QString &time);

    QByteArray set_sign_up_message(const QString &phone_number, const QString &first_name, const QString &last_name, const QString &password, const QString &secret_question, const QString &secret_answer);
    QByteArray set_login_request_message(const QString &phone_number, const QString &password);

    QByteArray set_delete_message(const int &conversation_ID, const QString &sender, const QString &receiver, const QString &time);

    QByteArray set_new_group_message(const QString &adm, const QStringList &members, const QString &group_name);

    QByteArray set_group_is_typing(const int &group_ID, const QString &group_name, const QString &sender);
    QByteArray set_group_text_message(const int &group_ID, const QString &group_name, const QString &sender, const QString &message, const QString &time);
    QByteArray set_group_file_message(const int &group_ID, const QString &group_name, const QString &sender, const QString &file_name, const QByteArray &file_data, const QString &time);
    QByteArray set_group_audio_message(const int &group_ID, const QString &group_name, const QString &sender, const QString &audio_name, const QString &time);

    QByteArray set_new_group_member_message(const int &group_ID, const QString &group_name, const QString &adm, const QString &group_member);
    QByteArray set_remove_group_member_message(const int &group_ID, const QString &group_name, const QString &adm, const QString &group_member);

    QByteArray set_request_data_message(const int &conversation_ID, const QString &date_time, const QString &type);

    QByteArray set_delete_account_message(const QString &phone_number);

    void load_data(const QByteArray &data);

    message_type type() const;
    const QString &name() const;

    const QString &message() const;
    const QString &receiver() const;
    const QString &sender() const;

    const QString &file_name() const;
    const qint64 &file_size() const;
    const QByteArray &file_data() const;
    const QString &file_sender() const;

    const QString &group_file_name() const;
    const qint64 &group_file_size() const;
    const QByteArray &group_file_data() const;
    const QString &group_file_sender() const;

    const QString &my_ID() const;
    const QString &my_name() const;

    const QString &clients_ID() const;
    const QString &client_name() const;
    const QString &old_name() const;

    const QString &audio_name() const;
    const QString &audio_sender() const;
    const QByteArray &audio_data() const;

    const QString &group_audio_name() const;
    const QString &group_audio_sender() const;
    const QString &group_data_type() const;
    const QByteArray &group_audio_data() const;

    const QStringList &online_friends() const;
    const QHash<int, QHash<QString, int>> &friend_list() const;
    const int &conversation_ID() const;
    const QHash<int, QStringList> &messages() const;

    const QString &hashed_password() const;
    const bool &true_or_false() const;

    const QString &time() const;

    const int &group_ID() const;
    const QString &adm() const;
    const QString &group_message() const;
    const QString &group_time() const;
    const QString &group_sender() const;
    const QStringList &group_members() const;
    const QString &group_name() const;

    const QHash<int, QHash<int, QString>> &group_list() const;
    const QHash<int, QStringList> &group_messages() const;
    const QHash<int, QStringList> &groups_members() const;

private:
    QByteArray get_data(message_type type, const QString &data);

    QString _my_name;
    QString _my_ID;

    message_type _type;
    QString _name;

    QString _message;
    QString _receiver;
    QString _sender;

    QString _file_name;
    qint64 _file_size;
    QByteArray _file_data;
    QString _file_sender;

    QString _group_file_name;
    qint64 _group_file_size;
    QByteArray _group_file_data;
    QString _group_file_sender;

    QString _client_ID;
    QString _client_name;
    QString _old_name;

    QHash<int, QHash<QString, int>> _friend_list;
    QStringList _online_friends;
    int _conversation_ID;
    QHash<int, QStringList> _messages;

    QString _audio_name;
    QString _audio_sender;
    QByteArray _audio_data;

    QString _group_audio_name;
    QString _group_audio_sender;
    QByteArray _group_audio_data;
    QString _group_data_type;

    QString _hashed_password;
    bool _true_or_false;

    QString _time;

    int _group_ID;
    QString _adm;
    QString _group_message;
    QString _group_time;
    QStringList _group_members;
    QString _group_name;
    QString _group_sender;
    QHash<int, QHash<int, QString>> _group_list;
    QHash<int, QStringList> _group_messages;
    QHash<int, QStringList> _groups_members;
};