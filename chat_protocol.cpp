#include "chat_protocol.h"

chat_protocol::chat_protocol(QWidget *parent)
    : QMainWindow(parent) {}
QByteArray chat_protocol::get_data(message_type type, QString data)
{
    QByteArray byte;

    QDataStream out(&byte, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);

    out << type << data;

    return byte;
}

QByteArray chat_protocol::set_text_message(QString sender, QString receiver, QString message)
{
    QByteArray byte;

    QDataStream out(&byte, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);

    out << text << sender << receiver << message;

    return byte;
}

QByteArray chat_protocol::set_is_typing_message(QString sender, QString receiver)
{
    QByteArray byte;

    QDataStream out(&byte, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);

    out << is_typing << sender << receiver;

    return byte;
}

QByteArray chat_protocol::set_name_message(QString my_name, QString name)
{
    QByteArray byte;

    QDataStream out(&byte, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);

    out << set_name << my_name << name;

    return byte;
}

QByteArray chat_protocol::set_init_sending_file_message(QString filename)
{
    QByteArray byte;

    QDataStream out(&byte, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);

    QFileInfo info(filename);

    out << init_sending_file << info.fileName() << info.size();

    return byte;
}

QByteArray chat_protocol::set_init_sending_file_message_client(QString sender, QString ID, QString receiver, QString filename)
{
    QByteArray byte;

    QDataStream out(&byte, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);

    QFileInfo info(filename);

    out << init_sending_file_client << sender << ID << receiver << info.fileName() << info.size();

    return byte;
}

QByteArray chat_protocol::set_accept_file_message()
{
    return get_data(accept_sending_file, "");
}

QByteArray chat_protocol::set_accept_file_message_client(QString receiver, int port)
{
    QByteArray byte;

    QDataStream out(&byte, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);

    out << accept_sending_file_client << receiver << port;

    return byte;
}

QByteArray chat_protocol::set_reject_file_message()
{
    return get_data(reject_sending_file, "");
}

QByteArray chat_protocol::set_reject_file_message_client(QString sender, QString receiver)
{
    QByteArray byte;

    QDataStream out(&byte, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);

    out << reject_sending_file_client << sender << receiver;

    return byte;
}

QByteArray chat_protocol::set_file_message(QString file_name)
{
    QByteArray byte;

    QFile file(file_name);
    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream out(&byte, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_0);

        QFileInfo info(file_name);

        out << send_file << info.fileName() << info.size() << file.readAll();

        file.close();
    }

    return byte;
}

QByteArray chat_protocol::set_file_message_client(QString filename, QString sender)
{
    QByteArray byte;

    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream out(&byte, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_0);

        QFileInfo info(filename);

        out << send_file_client << info.fileName() << info.size() << file.readAll() << sender;

        file.close();
    }

    return byte;
}

QByteArray chat_protocol::set_lookup_friend_message(QString ID)
{
    return get_data(lookup_friend, ID);
}

QByteArray chat_protocol::set_create_conversation_message(int conversation_ID, QString participant1, int participant1_ID, QString participant2, int participant2_ID)
{
    QByteArray byte;

    QDataStream out(&byte, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);

    out << create_conversation << conversation_ID << participant1 << participant1_ID << participant2 << participant2_ID;

    return byte;
}

QByteArray chat_protocol::set_audio_message(QString sender, QString receiver, QString audio_name)
{
    QByteArray byte;

    QFile file(audio_name);
    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream out(&byte, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_0);

        QFileInfo info(audio_name);

        out << audio << sender << receiver << info.fileName() << file.readAll();

        file.close();
    }
    else
        qDebug() << "chat_protocol ---> set_audio_message() ---> Can't open the file you wanna send";

    return byte;
}

QByteArray chat_protocol::set_save_message_message(int conversation_ID, QString sender, QString receiver, QString content)
{
    QByteArray byte;

    QDataStream out(&byte, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);

    out << save_message << conversation_ID << sender << receiver << content;

    return byte;
}

QByteArray chat_protocol::set_sign_in_message(QString phone_number, QString first_name, QString last_name, QString password, QString secret_question, QString secret_answer)
{
    QByteArray byte;

    QDataStream out(&byte, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);

    out << sign_in << phone_number << first_name << last_name << password << secret_question << secret_answer;

    return byte;
}

QByteArray chat_protocol::set_login_request_message(QString phone_number, QString password)
{
    QByteArray byte;

    QDataStream out(&byte, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);

    out << login_request << phone_number << password;

    return byte;
}

QByteArray chat_protocol::set_save_data_message(int conversation_ID, QString sender, QString receiver, QString file_name, QString type)
{
    QByteArray byte;

    QFile file(file_name);
    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream out(&byte, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_0);

        QFileInfo info(file_name);

        out << save_data << conversation_ID << sender << receiver << info.fileName() << file.readAll() << type;

        file.close();
    }
    else
        qDebug() << "chat_protocol ---> set_save_file_message() ---> Can't open the file you wanna send";

    return byte;
}

void chat_protocol::load_data(QByteArray data)
{
    QDataStream in(&data, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_6_0);

    in >> _type;

    switch (_type)
    {
    case text:
        in >> _sender >> _receiver >> _message;

        break;

    case set_name:
        in >> _name;

        break;

    case is_typing:
        in >> _sender >> _receiver;

        break;

    case init_sending_file:
        in >> _file_name >> _file_size;

        break;

    case init_sending_file_client:
        in >> _sender >> _client_ID >> _file_name_client >> _file_size_client;

        break;

    case send_file:
        in >> _file_name >> _file_size >> _file_data;

        break;

    case send_file_client:
        in >> _file_name_client >> _file_size_client >> _file_data_client >> _sender_file;

        break;

    case client_disconnected:
        in >> _client_name;

        break;

    case client_connected:
        in >> _client_name;

        break;

    case client_new_name:
        in >> _old_name >> _client_name;

        break;

    case accept_sending_file_client:
        in >> _port_transfer;

        break;

    case reject_sending_file_client:
        in >> _sender;

        break;

    case added_you:
        in >> _conversation_ID >> _client_name >> _client_ID >> _receiver;

        break;

    case lookup_friend:
        in >> _conversation_ID >> _client_name;

        break;

    case audio:
        in >> _audio_sender >> _audio_name >> _audio_data;

        break;

    case login_request:
        in >> _hashed_password >> _true_or_false >> _my_name >> _port >> _friend_list >> _online_friends >> _messages >> _binary_data;

        break;

    default:
        break;
    }
}

chat_protocol::message_type chat_protocol::type() const
{
    return _type;
}

const QString &chat_protocol::message() const
{
    return _message;
}

const QString &chat_protocol::name() const
{
    return _name;
}

const QString &chat_protocol::file_name() const
{
    return _file_name;
}

const qint64 &chat_protocol::file_size() const
{
    return _file_size;
}

const QByteArray &chat_protocol::file_data() const
{
    return _file_data;
}

const QString &chat_protocol::file_name_client() const
{
    return _file_name_client;
}

const qint64 &chat_protocol::file_size_client() const
{
    return _file_size_client;
}

const QByteArray &chat_protocol::file_data_client() const
{
    return _file_data_client;
}

const QString &chat_protocol::client_name() const
{
    return _client_name;
}

const QString &chat_protocol::receiver() const
{
    return _receiver;
}

const QString &chat_protocol::sender() const
{
    return _sender;
}

const QString &chat_protocol::sender_file() const
{
    return _sender_file;
}

const QString &chat_protocol::old_name() const
{
    return _old_name;
}

const QString &chat_protocol::my_name() const
{
    return _my_name;
}

const int &chat_protocol::port() const
{
    return _port;
}

const int &chat_protocol::port_transfer() const
{
    return _port_transfer;
}

const QString &chat_protocol::my_ID() const
{
    return _my_ID;
}

const QString &chat_protocol::clients_ID() const
{
    return _client_ID;
}

const QHash<int, QHash<QString, int>> &chat_protocol::friend_list() const
{
    return _friend_list;
}

const QHash<int, QVector<QString>> &chat_protocol::messages() const
{
    return _messages;
}

const QHash<int, QHash<QString, QByteArray>> &chat_protocol::binary_data() const
{
    return _binary_data;
}

const int &chat_protocol::conversation_ID() const
{
    return _conversation_ID;
}

const QString &chat_protocol::audio_name() const
{
    return _audio_name;
}

const QByteArray &chat_protocol::audio_data() const
{
    return _audio_data;
}

const QString &chat_protocol::audio_sender() const
{
    return _audio_sender;
}

const QList<QString> &chat_protocol::online_friends() const
{
    return _online_friends;
}

const QString &chat_protocol::hashed_password() const
{
    return _hashed_password;
}

const bool &chat_protocol::true_or_false() const
{
    return _true_or_false;
}