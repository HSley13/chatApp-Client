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

        mount_audio_IDBFS();
        mount_file_IDBFS();
    }
}

/*-------------------------------------------------------------------- Slots --------------------------------------------------------------*/

void client_manager::on_binary_message_received(const QByteArray &message)
{
    _protocol->load_data(message);

    switch (_protocol->type())
    {
    case chat_protocol::text:
        emit text_message_received(_protocol->sender(), _protocol->message(), _protocol->time());

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
        save_audio(_protocol->audio_sender(), _protocol->audio_name(), _protocol->audio_data(), _protocol->time());

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
        save_file(_protocol->file_sender(), _protocol->file_name(), _protocol->file_data(), _protocol->time());

        break;

    case chat_protocol::delete_message:
        delete_message(_protocol->sender(), _protocol->time());

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

void client_manager::send_text(QString sender, QString receiver, QString text, QString time)
{
    _socket->sendBinaryMessage(_protocol->set_text_message(sender, receiver, text, time));
}

void client_manager::send_name(QString name)
{
    _socket->sendBinaryMessage(_protocol->set_name_message(_my_ID, name));
}

void client_manager::send_is_typing(QString sender, QString receiver)
{
    _socket->sendBinaryMessage(_protocol->set_is_typing_message(sender, receiver));
}

void client_manager::save_file(QString sender, QString file_name, QByteArray file_data, QString time)
{
    QString full_file_name = QString("%1_%2").arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"), file_name);

    IDBFS_save_file(full_file_name, file_data, static_cast<int>(file_data.size()));

    emit file_received(sender, full_file_name, time);
}

void client_manager::send_save_audio(int conversation_ID, QString sender, QString receiver, QString data_name, QString type, QString time)
{
    _socket->sendBinaryMessage(_protocol->set_save_audio_message(conversation_ID, sender, receiver, data_name, type, time));
}

void client_manager::send_save_file(int conversation_ID, QString sender, QString receiver, QString data_name, QByteArray file_data, QString type, QString time)
{
    _socket->sendBinaryMessage(_protocol->set_save_file_message(conversation_ID, sender, receiver, data_name, file_data, type, time));
}

void client_manager::save_audio(QString sender, QString audio_name, QByteArray audio_data, QString time)
{
    QString full_audio_name = QString("%1_%2").arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"), audio_name);

    IDBFS_save_audio(full_audio_name, audio_data, static_cast<int>(audio_data.size()));

    emit audio_received(sender, full_audio_name, time);
}

void client_manager::send_audio(QString sender, QString receiver, QString audio_name, QString time)
{
    _socket->sendBinaryMessage(_protocol->set_audio_message(sender, receiver, audio_name, time));
}

void client_manager::send_lookup_friend(QString ID)
{
    _socket->sendBinaryMessage(_protocol->set_lookup_friend_message(ID));
}

void client_manager::send_create_conversation(int conversation_ID, QString participant1, int participant1_ID, QString participant2, int participant2_ID)
{
    _socket->sendBinaryMessage(_protocol->set_create_conversation_message(conversation_ID, participant1, participant1_ID, participant2, participant2_ID));
}

void client_manager::send_save_conversation(int conversation_ID, QString sender, QString receiver, QString content, QString time)
{
    _socket->sendBinaryMessage(_protocol->set_save_message_message(conversation_ID, sender, receiver, content, time));
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

void client_manager::send_file(QString sender, QString receiver, QString file_name, QByteArray file_data, QString time)
{
    _file_name = file_name;

    _socket->sendBinaryMessage(_protocol->set_file_message(sender, receiver, file_name, file_data, time));
}

void client_manager::send_delete_message(int conversation_ID, QString sender, QString receiver, QString time)
{
    _socket->sendBinaryMessage(_protocol->set_delete_message(conversation_ID, sender, receiver, time));
}

void client_manager::mount_audio_IDBFS()
{
    EM_ASM({
        FS.mkdir('/audio');
        FS.mount(IDBFS, {}, '/audio');
        FS.syncfs(true, function(err) {
            assert(!err);
            console.log('IDBFS audio mounted and synced'); });
    });
}

void client_manager::mount_file_IDBFS()
{
    EM_ASM({
        FS.mkdir('/file');
        FS.mount(IDBFS, {}, '/file');
        FS.syncfs(true, function(err) {
            assert(!err);
            console.log('IDBFS file mounted and synced'); });
    });
}

void client_manager::IDBFS_save_audio(QString audio_name, QByteArray audio_data, int size)
{
    std::string audio_path = "/audio/";
    audio_path += audio_name.toStdString();

    FILE *file = fopen(audio_path.c_str(), "wb");
    if (file)
    {
        fwrite(audio_data, 1, size, file);
        fclose(file);
    }
    else
        qDebug() << "Failed to open audio for writing:" << QString::fromStdString(audio_path);
}

void client_manager::IDBFS_save_file(QString file_name, QByteArray file_data, int size)
{
    std::string file_path = "/file/";
    file_path += file_name.toStdString();

    FILE *file = fopen(file_path.c_str(), "wb");
    if (file)
    {
        fwrite(file_data, 1, size, file);
        fclose(file);
    }
    else
        qDebug() << "Failed to open file for writing:" << QString::fromStdString(file_path);
}

QUrl client_manager::get_audio_url(const QString &audio_name)
{
    const QString full_audio_path = "/audio/" + audio_name;

    const char *c_audio_name = full_audio_path.toUtf8().constData();

    char *url = (char *)EM_ASM_PTR(
        {
            var audio_path = UTF8ToString($0);
            var audio_data = FS.readFile(audio_path);

            if (!audio_data)
            {
                console.error("Failed to read file:", audio_path);
                return null;
            }

            var blob = new Blob([audio_data],
                                { type: 'audio/*' });
            var url = URL.createObjectURL(blob);

            var url_length = lengthBytesUTF8(url) + 1;
            var stringOnWasmHeap = _malloc(url_length);

            stringToUTF8(url, stringOnWasmHeap, url_length);

            return stringOnWasmHeap;
        },
        c_audio_name);

    if (!url)
    {
        qDebug() << "client_manager ---> get_audio_url() ---> url empty";
        return QUrl();
    }

    QString qUrl = QString::fromUtf8(url);
    free(url);

    return QUrl(qUrl);
}

QUrl client_manager::get_file_url(const QString &file_name)
{
    const QString full_file_path = "/file/" + file_name;

    const char *c_filename = full_file_path.toUtf8().constData();

    char *url = (char *)EM_ASM_PTR(
        {
            var file_path = UTF8ToString($0);
            var file_data = FS.readFile(file_path);

            if (!file_data)
            {
                console.error("Failed to read file:", file_path);
                return null;
            }

            var mime_type = 'application/octet-stream';
            var extension = file_path.split('.').pop().toLowerCase();

            switch (extension)
            {
            case 'pdf':
                mime_type = 'application/pdf';
                break;
            case 'jpg':
            case 'jpeg':
            case 'png':
            case 'webp':
            case 'gif':
            case 'bmp':
            case 'svg':
                mime_type = 'image/*';
                break;
            case 'txt':
                mime_type = 'text/plain';
                break;
            case 'html':
            case 'htm':
                mime_type = 'text/html';
                break;
            case 'css':
                mime_type = 'text/css';
                break;
            case 'js':
                mime_type = 'application/javascript';
                break;
            case 'json':
                mime_type = 'application/json';
                break;
            case 'xml':
                mime_type = 'application/xml';
                break;
            case 'csv':
                mime_type = 'text/csv';
                break;
            case 'doc':
                mime_type = 'application/msword';
                break;
            case 'docx':
                mime_type = 'application/vnd.openxmlformats-officedocument.wordprocessingml.document';
                break;
            case 'xls':
                mime_type = 'application/vnd.ms-excel';
                break;
            case 'xlsx':
                mime_type = 'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet';
                break;
            case 'ppt':
                mime_type = 'application/vnd.ms-powerpoint';
                break;
            case 'pptx':
                mime_type = 'application/vnd.openxmlformats-officedocument.presentationml.presentation';
                break;
            case 'mp4':
                mime_type = 'video/mp4';
                break;
            case 'avi':
                mime_type = 'video/x-msvideo';
                break;
            case 'mov':
                mime_type = 'video/quicktime';
                break;
            case 'zip':
                mime_type = 'application/zip';
                break;
            case 'rar':
                mime_type = 'application/vnd.rar';
                break;
            case 'tar':
                mime_type = 'application/x-tar';
                break;
            case '7z':
                mime_type = 'application/x-7z-compressed';
                break;
            case 'epub':
                mime_type = 'application/epub+zip';
                break;
            case 'mobi':
                mime_type = 'application/x-mobipocket-ebook';
                break;
            case 'azw':
                mime_type = 'application/vnd.amazon.ebook';
                break;
            case 'webm':
                mime_type = 'video/webm';
                break;
            case 'mkv':
                mime_type = 'video/x-matroska';
                break;
            case 'rtf':
                mime_type = 'application/rtf';
                break;
            case 'psd':
                mime_type = 'image/vnd.adobe.photoshop';
                break;
            case 'ai':
            case 'eps':
            case 'ps':
                mime_type = 'application/postscript';
                break;
            case 'tex':
                mime_type = 'application/x-tex';
                break;
            case 'latex':
                mime_type = 'application/x-latex';
                break;
            case 'md':
                mime_type = 'text/markdown';
                break;
            case 'log':
                mime_type = 'text/plain';
                break;
            case 'c':
            case 'cpp':
            case 'h':
            case 'hpp':
                mime_type = 'text/x-c';
                break;
            case 'py':
                mime_type = 'text/x-python';
                break;
            case 'java':
                mime_type = 'text/x-java-source';
                break;
            case 'sh':
                mime_type = 'application/x-sh';
                break;
            case 'bat':
                mime_type = 'application/x-msdos-program';
                break;
            case 'exe':
                mime_type = 'application/x-msdownload';
                break;
            default:
                console.warn("Unknown file extension:", extension);
                break;
            }

            var blob = new Blob([file_data],
                                { type: mime_type });
            var url = URL.createObjectURL(blob);

            var url_length = lengthBytesUTF8(url) + 1;
            var stringOnWasmHeap = _malloc(url_length);

            stringToUTF8(url, stringOnWasmHeap, url_length);

            return stringOnWasmHeap;
        },
        c_filename);

    if (!url)
    {
        qDebug() << "client_manager ---> get_file_url() ---> url empty";
        return QUrl();
    }

    QString qUrl = QString::fromUtf8(url);
    free(url);

    return QUrl(qUrl);
}
