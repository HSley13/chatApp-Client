#pragma once

#include "chat_protocol.h"
#include <QtWidgets>
#include <QtCore>
#include <QtMultimedia>
#include <QWebSocket>
#include <QWebSocketServer>
class client_manager : public QMainWindow
{
    Q_OBJECT
public:
    client_manager(QWidget *parent = nullptr);

    void send_text(QString sender, QString receiver, QString text);
    void send_name(QString name);
    void send_is_typing(QString sender, QString receiver);

    void send_init_sending_file(QString filename);
    void send_accept_file();
    void send_reject_file();

    void send_init_sending_file_client(QString sender, QString receiver, QString filename);
    void send_accept_file_client(QString receiver);
    void send_reject_file_client(QString sender, QString receiver);

    void send_file();
    void send_file_client(int port_transfer);

    void save_file(QString date_time);
    void save_file_client(QString sender, QString file_name, QByteArray file_data, QString date_time);

    void save_audio(QString sender, QString file_name, QByteArray file_data, QString date_time);
    void send_audio_message(QString sender, QString receiver, QString audio_name);

    void send_lookup_friend(QString ID);

    void send_create_conversation_message(int conversation_ID, QString participant1, int participant1_ID, QString participant2, int participant2_ID);

    void send_save_conversation_message(int conversation_ID, QString sender, QString receiver, QString content);

    void send_save_data_message(int conversation_ID, QString sender, QString receiver, QString type);

    void send_save_audio_message(int conversation_ID, QString sender, QString receiver, QString audio_name, QString type);

    void send_sign_in_message(QString phone_number, QString first_name, QString last_name, QString password, QString secret_question, QString secret_answer);

    void send_login_request_message(QString phone_number, QString password);

    static QString _my_ID;
    static QString _my_name;
    QString _file_name;
    QString _file_name_client;

private:
    static QWebSocket *_socket;
    QHostAddress _ip = QHostAddress::LocalHost;
    int _port = 12345;

    static chat_protocol *_protocol;

    static QWebSocketServer *_file_server;
    QWebSocket *_file_socket;

signals:
    void text_message_received(QString sender, QString message);
    void is_typing_received(QString sender);

    void init_receiving_file(QString file_name, qint64 file_size);
    void reject_receiving_file();

    void init_receiving_file_client(QString sender, QString ID, QString file_name, qint64 file_size);
    void reject_receiving_file_client(QString sender);

    void file_saved(QString sender, QString path);

    void client_name_changed(QString old_name, QString client_name);
    void client_connected(QString client_name);
    void client_disconnected(QString client_name);

    void socket_disconnected();

    void file_accepted();
    void file_accepted_client();

    void audio_received(QString sender, QString path);

    void client_added_you(int conversation_ID, QString name, QString ID);
    void lookup_friend_result(int conversation_ID, QString name);

    void login_request(QString hashed_password, bool true_or_false, QHash<int, QHash<QString, int>> friend_list, QList<QString> online_friends, QHash<int, QVector<QString>> messages, QHash<int, QHash<QString, QByteArray>> binary_data);

private slots:
    void on_new_connection();

    void on_disconnected();

    void on_binary_message_received(const QByteArray &message);

    void file_connect(const QByteArray &message);
};