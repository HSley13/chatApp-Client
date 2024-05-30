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

    void send_text(QString sender, QString receiver, QString text);
    void send_name(QString name);
    void send_is_typing(QString sender, QString receiver);

    void save_file(QString sender, QString file_name, QByteArray file_data, QString date_time);
    void save_audio(QString sender, QString audio_name, QByteArray audio_data, QString date_time);

    void send_audio(QString sender, QString receiver, QString audio_name);
    void send_lookup_friend(QString ID);

    void send_create_conversation(int conversation_ID, QString participant1, int participant1_ID, QString participant2, int participant2_ID);
    void send_save_conversation(int conversation_ID, QString sender, QString receiver, QString content);

    void send_save_audio(int conversation_ID, QString sender, QString receiver, QString data_name, QString type);
    void send_save_file(int conversation_ID, QString sender, QString receiver, QString data_name, QByteArray file_data, QString type);

    void send_sign_up(QString phone_number, QString first_name, QString last_name, QString password, QString secret_question, QString secret_answer);
    void send_login_request(QString phone_number, QString password);

    void send_init_send_file(QString sender, QString my_ID, QString receiver, QString file_name, qint64 file_size);
    void send_file_accepted(QString sender, QString receiver);
    void send_file_rejected(QString sender, QString receiver);
    void send_file(QString sender, QString receiver, QString file_name, QByteArray file_data);

    void mount_audio_IDBFS();
    void mount_file_IDBFS();

    void IDBFS_save_audio(QString audio_name, QByteArray audio_data, int size);
    QUrl get_audio_url(const QString &audio_name);

    void IDBFS_save_file(QString file_name, QByteArray file_data, int size);
    QUrl get_file_url(const QString &file_name);

    static QString _my_ID;
    static QString _my_name;
    QString _file_name;

private:
    static QWebSocket *_socket;

    static chat_protocol *_protocol;

    static QWebSocketServer *_file_server;
    QWebSocket *_file_socket;

signals:
    void text_message_received(QString sender, QString message);
    void is_typing_received(QString sender);

    void client_name_changed(QString old_name, QString client_name);
    void client_connected(QString client_name);
    void client_disconnected(QString client_name);

    void socket_disconnected();

    void audio_received(QString sender, QString audio);
    void file_received(QString sender, QString file_name);

    void client_added_you(int conversation_ID, QString name, QString ID);
    void lookup_friend_result(int conversation_ID, QString name, bool true_or_false);

    void login_request(QString hashed_password, bool true_or_false, QHash<int, QHash<QString, int>> friend_list, QList<QString> online_friends, QHash<int, QVector<QString>> messages, QHash<int, QHash<QString, QByteArray>> binary_data);

    void init_send_file_received(QString sender, QString sender_ID, QString file_name, qint64 file_size);
    void file_accepted(QString sender);
    void file_rejected(QString sender);

private slots:
    void on_disconnected();

    void on_binary_message_received(const QByteArray &message);
};
