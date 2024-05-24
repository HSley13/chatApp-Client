#include "client_manager.h"

QWebSocket *client_manager::_socket = nullptr;
chat_protocol *client_manager::_protocol = nullptr;

QWebSocketServer *client_manager::_file_server = nullptr;

QString client_manager::_my_ID;
QString client_manager::_my_name;

client_manager::client_manager(QWidget *parent)
    : QMainWindow(parent)
{
    if (!_socket && !_protocol)
    {
        _socket = new QWebSocket(QString(), QWebSocketProtocol::VersionLatest, this);

        QUrl url = QUrl(QString("wss://chatapp.hslay13.online"));
        _socket->open(url);

        connect(_socket, &QWebSocket::disconnected, this, &client_manager::on_disconnected);
        connect(_socket, &QWebSocket::binaryMessageReceived, this, &client_manager::on_binary_message_received);

        _protocol = new chat_protocol(this);
    }
}

/*-------------------------------------------------------------------- Slots --------------------------------------------------------------*/

void client_manager::on_binary_message_received(const QByteArray &message)
{
    _protocol->load_data(message);

    switch (_protocol->type())
    {
    case chat_protocol::text:
        emit text_message_received(_protocol->sender(), _protocol->message());

        break;

    case chat_protocol::is_typing:
        emit is_typing_received(_protocol->sender());

        break;

    case chat_protocol::client_new_name:
        emit client_name_changed(_protocol->old_name(), _protocol->client_name());

        break;

    case chat_protocol::client_disconnected:
        emit client_disconnected(_protocol->client_name());

        break;

    case chat_protocol::client_connected:
        emit client_connected(_protocol->client_name());

        break;

    case chat_protocol::added_you:
        emit client_added_you(_protocol->conversation_ID(), _protocol->client_name(), _protocol->clients_ID());

        break;

    case chat_protocol::lookup_friend:
        emit lookup_friend_result(_protocol->conversation_ID(), _protocol->client_name(), _protocol->true_or_false());

        break;

    case chat_protocol::audio:
        save_audio(_protocol->audio_sender(), _protocol->audio_name(), _protocol->audio_data(), QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"));

        break;

    case chat_protocol::login_request:
    {
        emit login_request(_protocol->hashed_password(), _protocol->true_or_false(), _protocol->friend_list(), _protocol->online_friends(), _protocol->messages(), _protocol->binary_data());

        _my_name = _protocol->my_name();
    }

    break;

    case chat_protocol::init_send_file:
        emit init_send_file_received(_protocol->file_sender(), _protocol->clients_ID(), _protocol->file_name(), _protocol->file_size());

        break;

    case chat_protocol::file_accepted:
        emit file_accepted(_protocol->file_sender());

        break;

    case chat_protocol::file_rejected:
        emit file_rejected(_protocol->file_sender());

        break;

    case chat_protocol::file:
        save_file(_protocol->file_sender(), _protocol->file_name(), _protocol->file_data(), QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"));

        break;

    default:
        break;
    }
}

void client_manager::on_disconnected()
{
    emit socket_disconnected();
}

/*-------------------------------------------------------------------- Functions --------------------------------------------------------------*/

void client_manager::send_text(QString sender, QString receiver, QString text)
{
    _socket->sendBinaryMessage(_protocol->set_text_message(sender, receiver, text));
}

void client_manager::send_name(QString name)
{
    _socket->sendBinaryMessage(_protocol->set_name_message(_my_ID, name));
}

void client_manager::send_is_typing(QString sender, QString receiver)
{
    _socket->sendBinaryMessage(_protocol->set_is_typing_message(sender, receiver));
}

void client_manager::save_file(QString sender, QString file_name, QByteArray file_data, QString date_time)
{
#ifdef Q_OS_WASM
    QFileDialog::saveFileContent(file_data, file_name);
    emit file_received(sender, "");
#else
    QDir dir;
    if (!sender.isEmpty() && !sender.isNull())
        dir.mkdir(sender);

    QString path = QString("%1/%2/%3_%4").arg(dir.canonicalPath(), sender, date_time, file_name);

    QFile file(path);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(file_data);
        file.close();

        emit file_received(sender, path);
    }
    else
        qDebug() << "client_manager ---> save_file() ---> Couldn't open the file to write to it";
#endif
}

void client_manager::send_save_data(int conversation_ID, QString sender, QString receiver, QString data_name, QString type)
{
    _socket->sendBinaryMessage(_protocol->set_save_data_message(conversation_ID, sender, receiver, data_name, type));
}

void client_manager::save_audio(QString sender, QString file_name, QByteArray file_data, QString date_time)
{
    QDir dir;
    if (!sender.isEmpty() && !sender.isNull())
        dir.mkdir(sender);

    QString output_directory = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString output_location = output_directory + "/" + date_time + "_" + file_name;

    QFile file(output_location);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(file_data);
        file.close();

        emit audio_received(sender, QUrl::fromLocalFile(output_location));
    }
    else
        qDebug() << "client_manager ---> save_audio() ---> Couldn't open the file to write to it";
}

void client_manager::send_audio(QString sender, QString receiver, QString audio_name)
{
    _socket->sendBinaryMessage(_protocol->set_audio_message(sender, receiver, audio_name));
}

void client_manager::send_lookup_friend(QString ID)
{
    _socket->sendBinaryMessage(_protocol->set_lookup_friend_message(ID));
}

void client_manager::send_create_conversation(int conversation_ID, QString participant1, int participant1_ID, QString participant2, int participant2_ID)
{
    _socket->sendBinaryMessage(_protocol->set_create_conversation_message(conversation_ID, participant1, participant1_ID, participant2, participant2_ID));
}

void client_manager::send_save_conversation(int conversation_ID, QString sender, QString receiver, QString content)
{
    _socket->sendBinaryMessage(_protocol->set_save_message_message(conversation_ID, sender, receiver, content));
}

void client_manager::send_sign_up(QString phone_number, QString first_name, QString last_name, QString password, QString secret_question, QString secret_answer)
{
    _socket->sendBinaryMessage(_protocol->set_sign_up_message(phone_number, first_name, last_name, password, secret_question, secret_answer));
}

void client_manager::send_login_request(QString phone_number, QString password)
{
    _my_ID = phone_number;
    _socket->sendBinaryMessage(_protocol->set_login_request_message(phone_number, password));
}

void client_manager::send_init_send_file(QString sender, QString my_ID, QString receiver, QString file_name, qint64 file_size)
{
    _socket->sendBinaryMessage(_protocol->set_init_send_file_message(sender, my_ID, receiver, file_name, file_size));
}

void client_manager::send_file_accepted(QString sender, QString receiver)
{
    _socket->sendBinaryMessage(_protocol->set_file_accepted_message(sender, receiver));
}

void client_manager::send_file_rejected(QString sender, QString receiver)
{
    _socket->sendBinaryMessage(_protocol->set_file_rejected_message(sender, receiver));
}

void client_manager::send_file(QString sender, QString receiver, QString file_name, QByteArray file_data)
{
    _file_name = file_name;
    _socket->sendBinaryMessage(_protocol->set_file_message(sender, receiver, file_name, file_data));
}