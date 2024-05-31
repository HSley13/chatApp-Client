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

        init_send_file,
        file_accepted,
        file_rejected,
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

        delete_message
    };

    QByteArray set_text_message(QString sender, QString receiver, QString message, QString time);
    QByteArray set_is_typing_message(QString sender, QString receiver);
    QByteArray set_name_message(QString my_name, QString name);

    QByteArray set_init_send_file_message(QString sender, QString my_ID, QString receiver, QString filename, qint64 file_size);
    QByteArray set_file_accepted_message(QString sender, QString receiver);
    QByteArray set_file_rejected_message(QString sender, QString receiver);

    QByteArray set_file_message(QString sender, QString receiver, QString file_name, QByteArray file_data, QString time);
    QByteArray set_audio_message(QString sender, QString receiver, QString audio_name, QString time);
    QByteArray set_save_audio_message(int conversation_ID, QString sender, QString receiver, QString file_name, QString type, QString time);
    QByteArray set_save_file_message(int conversation_ID, QString sender, QString receiver, QString file_name, QByteArray file_data, QString type, QString time);

    QByteArray set_lookup_friend_message(QString ID);
    QByteArray set_create_conversation_message(int conversation_ID, QString participant1, int participant1_ID, QString participant2, int participant2_ID);
    QByteArray set_save_message_message(int conversation_ID, QString sender, QString receiver, QString content, QString time);

    QByteArray set_sign_up_message(QString phone_number, QString first_name, QString last_name, QString password, QString secret_question, QString secret_answer);
    QByteArray set_login_request_message(QString phone_number, QString password);

    QByteArray set_delete_message(const int conversation_ID, QString sender, QString receiver, QString time);

    void load_data(QByteArray data);

    message_type type() const;
    const QString &name() const;

    const QString &message() const;
    const QString &receiver() const;
    const QString &sender() const;

    const QString &file_name() const;
    const qint64 &file_size() const;
    const QByteArray &file_data() const;
    const QString &file_sender() const;

    const QString &my_ID() const;
    const QString &my_name() const;

    const QString &clients_ID() const;
    const QString &client_name() const;
    const QString &old_name() const;

    const QString &audio_name() const;
    const QString &audio_sender() const;
    const QByteArray &audio_data() const;

    const QList<QString> &online_friends() const;
    const QHash<int, QHash<QString, int>> &friend_list() const;
    const int &conversation_ID() const;
    const QHash<int, QVector<QString>> &messages() const;
    const QHash<int, QHash<QString, QByteArray>> &binary_data() const;

    const QString &hashed_password() const;
    const bool &true_or_false() const;

    const QString &time() const;

private:
    QByteArray get_data(message_type type, QString data);

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

    QString _client_ID;
    QString _client_name;
    QString _old_name;

    QHash<int, QHash<QString, int>> _friend_list;
    QList<QString> _online_friends;
    int _conversation_ID;
    QHash<int, QVector<QString>> _messages;
    QHash<int, QHash<QString, QByteArray>> _binary_data;

    QString _audio_name;
    QString _audio_sender;
    QByteArray _audio_data;

    QString _hashed_password;
    bool _true_or_false;

    QString _time;
};