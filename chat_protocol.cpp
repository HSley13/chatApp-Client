#include "chat_protocol.h"

chat_protocol::chat_protocol(QWidget *parent)
    : QMainWindow(parent) {}

void chat_protocol::load_data(const QByteArray &data)
{
    QBuffer buffer;
    buffer.setData(data);
    buffer.open(QIODevice::ReadOnly);

    QDataStream in(&buffer);
    in.setVersion(QDataStream::Qt_6_7);

    in >> _type;

    switch (_type)
    {
    case text:
        in >> _sender >> _receiver >> _message >> _time;

        break;

    case set_name:
        in >> _name;

        break;

    case is_typing:
        in >> _sender >> _receiver;

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

    case added_you:
        in >> _conversation_ID >> _client_name >> _client_ID >> _receiver;

        break;

    case lookup_friend:
        in >> _conversation_ID >> _client_name >> _true_or_false;

        break;

    case audio:
        in >> _audio_sender >> _audio_name >> _audio_data >> _time;

        break;

    case login_request:
        in >> _hashed_password >> _true_or_false >> _my_name >> _friend_list >> _online_friends >> _messages >> _binary_data >> _group_list >> _group_messages >> _group_binary_data >> _groups_members;

        break;

    case file:
        in >> _file_sender >> _file_name >> _file_data >> _time;

        break;

    case delete_message:
        in >> _sender >> _time;

        break;

    case new_group:
        in >> _group_ID;

        break;

    case added_to_group:
        in >> _group_ID >> _adm >> _group_members >> _group_name;

        break;

    case group_is_typing:
        in >> _group_ID >> _group_name >> _group_sender;

        break;

    case group_text:
        in >> _group_ID >> _group_name >> _group_sender >> _group_message >> _group_time;

        break;

    case group_audio:
        in >> _group_ID >> _group_name >> _group_sender >> _group_audio_name >> _group_audio_data >> _group_time;

        break;

    case group_file:
        in >> _group_ID >> _group_name >> _group_sender >> _group_file_name >> _group_file_data >> _group_time;

        break;

    case remove_group_member:
        in >> _group_ID >> _group_name >> _adm;

        break;

    default:
        break;
    }
}

QByteArray chat_protocol::get_data(message_type type, const QString &data)
{
    QByteArray byte;

    QDataStream out(&byte, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_7);

    out << type << data;

    return byte;
}

QByteArray chat_protocol::set_text_message(const QString &sender, const QString &receiver, const QString &message, const QString &time)
{
    QByteArray byte;

    QDataStream out(&byte, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_7);

    out << text << sender << receiver << message << time;

    return byte;
}

QByteArray chat_protocol::set_is_typing_message(const QString &sender, const QString &receiver)
{
    QByteArray byte;

    QDataStream out(&byte, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_7);

    out << is_typing << sender << receiver;

    return byte;
}

QByteArray chat_protocol::set_name_message(const QString &my_name, const QString &name)
{
    QByteArray byte;

    QDataStream out(&byte, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_7);

    out << set_name << my_name << name;

    return byte;
}

QByteArray chat_protocol::set_lookup_friend_message(const QString &ID)
{
    return get_data(lookup_friend, ID);
}

QByteArray chat_protocol::set_create_conversation_message(const int &conversation_ID, const QString &participant1, const int &participant1_ID, const QString &participant2, const int &participant2_ID)
{
    QByteArray byte;

    QDataStream out(&byte, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_7);

    out << create_conversation << conversation_ID << participant1 << participant1_ID << participant2 << participant2_ID;

    return byte;
}

QByteArray chat_protocol::set_audio_message(const QString &sender, const QString &receiver, const QString &audio_name, const QString &time)
{
    QByteArray byte;

    QFile file(audio_name);
    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream out(&byte, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_7);

        QFileInfo info(audio_name);

        out << audio << sender << receiver << info.fileName() << file.readAll() << time;

        file.close();
    }
    else
        qDebug() << "chat_protocol ---> set_audio_message() ---> Can't open the file you wanna send";

    return byte;
}

QByteArray chat_protocol::set_save_message_message(const int &conversation_ID, const QString &sender, const QString &receiver, const QString &content, const QString &time)
{
    QByteArray byte;

    QDataStream out(&byte, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_7);

    out << save_message << conversation_ID << sender << receiver << content << time;

    return byte;
}

QByteArray chat_protocol::set_sign_up_message(const QString &phone_number, const QString &first_name, const QString &last_name, const QString &password, const QString &secret_question, const QString &secret_answer)
{
    QByteArray byte;

    QDataStream out(&byte, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_7);

    out << sign_up << phone_number << first_name << last_name << password << secret_question << secret_answer;

    return byte;
}

QByteArray chat_protocol::set_login_request_message(const QString &phone_number, const QString &password)
{
    QByteArray byte;

    QDataStream out(&byte, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_7);

    out << login_request << phone_number << password;

    return byte;
}

QByteArray chat_protocol::set_save_audio_message(const int &conversation_ID, const QString &sender, const QString &receiver, const QString &file_name, const QString &type, const QString &time)
{
    QByteArray byte;

    QFile file(file_name);
    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream out(&byte, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_7);

        QFileInfo info(file_name);

        out << save_data << conversation_ID << sender << receiver << info.fileName() << file.readAll() << type << time;

        file.close();
    }
    else
        qDebug() << "chat_protocol ---> set_save_file_message() ---> Can't open the audio you wanna send";

    return byte;
}

QByteArray chat_protocol::set_save_file_message(const int &conversation_ID, const QString &sender, const QString &receiver, const QString &file_name, const QByteArray &file_data, const QString &type, const QString &time)
{
    QByteArray byte;

    QDataStream out(&byte, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_7);

    out << save_data << conversation_ID << sender << receiver << file_name << file_data << type << time;

    return byte;
}

QByteArray chat_protocol::set_file_message(const QString &sender, const QString &receiver, const QString &file_name, const QByteArray &file_data, const QString &time)
{
    QByteArray byte;

    QDataStream out(&byte, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_7);

    out << file << sender << receiver << file_name << file_data << time;

    return byte;
}

QByteArray chat_protocol::set_delete_message(const int &conversation_ID, const QString &sender, const QString &receiver, const QString &time)
{
    QByteArray byte;

    QDataStream out(&byte, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_7);

    out << delete_message << conversation_ID << sender << receiver << time;

    return byte;
}

QByteArray chat_protocol::set_new_group_message(const QString &adm, const QStringList &members, const QString &group_name)
{
    QByteArray byte;

    QDataStream out(&byte, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_7);

    out << new_group << adm << members << group_name;

    return byte;
}

QByteArray chat_protocol::set_group_is_typing(const int &group_ID, const QString &group_name, const QString &sender)
{
    QByteArray byte;

    QDataStream out(&byte, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_7);

    out << group_is_typing << group_ID << group_name << sender;

    return byte;
}

QByteArray chat_protocol::set_group_text_message(const int &group_ID, const QString &group_name, const QString &sender, const QString &message, const QString &time)
{
    QByteArray byte;

    QDataStream out(&byte, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_7);

    out << group_text << group_ID << group_name << sender << message << time;

    return byte;
}

QByteArray chat_protocol::set_group_file_message(const int &group_ID, const QString &group_name, const QString &sender, const QString &file_name, const QByteArray &file_data, const QString &time)
{
    QByteArray byte;

    QDataStream out(&byte, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_7);

    out << group_file << group_ID << group_name << sender << file_name << file_data << time;

    return byte;
}

QByteArray chat_protocol::set_group_audio_message(const int &group_ID, const QString &group_name, const QString &sender, const QString &audio_name, const QString &time)
{
    QByteArray byte;

    QFile file(audio_name);
    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream out(&byte, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_7);

        QFileInfo info(audio_name);

        out << group_audio << group_ID << group_name << sender << info.fileName() << file.readAll() << time;

        file.close();
    }
    else
        qDebug() << "chat_protocol ---> set_group_audio_message() ---> Can't open the file you wanna send";

    return byte;
}

QByteArray chat_protocol::set_new_group_member_message(const int &group_ID, const QString &group_name, const QString &adm, const QString &group_member)
{
    QByteArray byte;

    QDataStream out(&byte, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_7);

    out << new_group_member << group_ID << group_name << adm << group_member;

    return byte;
}

QByteArray chat_protocol::set_remove_group_member_message(const int &group_ID, const QString &group_name, const QString &adm, const QString &group_member)
{
    QByteArray byte;

    QDataStream out(&byte, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_7);

    out << remove_group_member << group_ID << group_name << adm << group_member;

    return byte;
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

const QString &chat_protocol::receiver() const
{
    return _receiver;
}

const QString &chat_protocol::sender() const
{
    return _sender;
}

const QString &chat_protocol::old_name() const
{
    return _old_name;
}

const QString &chat_protocol::my_name() const
{
    return _my_name;
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

const QHash<int, QStringList> &chat_protocol::messages() const
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

const QStringList &chat_protocol::online_friends() const
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

const QString &chat_protocol::client_name() const
{
    return _client_name;
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

const QString &chat_protocol::group_file_name() const
{
    return _group_file_name;
}

const qint64 &chat_protocol::group_file_size() const
{
    return _group_file_size;
}

const QByteArray &chat_protocol::group_file_data() const
{
    return _group_file_data;
}

const QString &chat_protocol::file_sender() const
{
    return _file_sender;
}

const QString &chat_protocol::time() const
{
    return _time;
}

const int &chat_protocol::group_ID() const
{
    return _group_ID;
}

const QString &chat_protocol::adm() const
{
    return _adm;
}

const QStringList &chat_protocol::group_members() const
{
    return _group_members;
}

const QString &chat_protocol::group_name() const
{
    return _group_name;
}

const QString &chat_protocol::group_audio_name() const
{
    return _group_audio_name;
}

const QByteArray &chat_protocol::group_audio_data() const
{
    return _group_audio_data;
}

const QString &chat_protocol::group_audio_sender() const
{
    return _group_audio_sender;
}

const QString &chat_protocol::group_data_type() const
{
    return _group_data_type;
}

const QString &chat_protocol::group_sender() const
{
    return _group_sender;
}

const QString &chat_protocol::group_message() const
{
    return _group_message;
}

const QString &chat_protocol::group_time() const
{
    return _group_time;
}

const QHash<int, QHash<int, QString>> &chat_protocol::group_list() const
{
    return _group_list;
}

const QHash<int, QStringList> &chat_protocol::group_messages() const
{
    return _group_messages;
}

const QHash<int, QHash<QString, QByteArray>> &chat_protocol::group_binary_data() const
{
    return _group_binary_data;
}

const QHash<int, QStringList> &chat_protocol::groups_members() const
{
    return _groups_members;
}