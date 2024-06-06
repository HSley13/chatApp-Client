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

    case chat_protocol::file:
        save_file(_protocol->file_sender(), _protocol->file_name(), _protocol->file_data(), _protocol->time());

        break;

    case chat_protocol::delete_message:
        delete_message(_protocol->sender(), _protocol->time());

        break;

    case chat_protocol::new_group:
        emit new_group_ID(_protocol->group_ID());

        break;

    case chat_protocol::added_to_group:
        emit added_to_group(_protocol->group_ID(), _protocol->adm(), _protocol->group_members(), _protocol->group_name());

        break;

    case chat_protocol::group_is_typing:
        emit group_is_typing_received(_protocol->group_ID(), _protocol->group_sender());

        break;

    case chat_protocol::group_text:
        emit group_text_received(_protocol->group_ID(), _protocol->group_sender(), _protocol->group_message(), _protocol->group_time());

        break;

    case chat_protocol::group_audio:
        save_group_audio(_protocol->group_ID(), _protocol->group_sender(), _protocol->group_audio_name(), _protocol->group_audio_data(), _protocol->group_time());

        break;

    case chat_protocol::group_file:
        save_group_file(_protocol->group_ID(), _protocol->group_sender(), _protocol->group_file_name(), _protocol->group_file_data(), _protocol->group_time());

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

void client_manager::send_text(const QString &sender, const QString &receiver, const QString &text, const QString &time)
{
    _socket->sendBinaryMessage(_protocol->set_text_message(sender, receiver, text, time));
}

void client_manager::send_name(const QString &name)
{
    _my_name = name;

    _socket->sendBinaryMessage(_protocol->set_name_message(_my_ID, name));
}

void client_manager::send_is_typing(const QString &sender, const QString &receiver)
{
    _socket->sendBinaryMessage(_protocol->set_is_typing_message(sender, receiver));
}

void client_manager::save_file(const QString &sender, const QString &file_name, const QByteArray &file_data, const QString &time)
{
    emit saving_file(QString("Saving file: %1").arg(file_name));

    QString full_file_name = QString("%1_%2").arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"), file_name);

    IDBFS_save_file(full_file_name, file_data, static_cast<int>(file_data.size()));

    emit file_received(sender, full_file_name, time);
}

void client_manager::send_save_audio(const int &conversation_ID, const QString &sender, const QString &receiver, const QString &data_name, const QString &type, const QString &time)
{
    _socket->sendBinaryMessage(_protocol->set_save_audio_message(conversation_ID, sender, receiver, data_name, type, time));
}

void client_manager::send_save_file(const int &conversation_ID, const QString &sender, const QString &receiver, const QString &data_name, const QByteArray &file_data, const QString &type, const QString &time)
{
    _socket->sendBinaryMessage(_protocol->set_save_file_message(conversation_ID, sender, receiver, data_name, file_data, type, time));
}

void client_manager::save_audio(const QString &sender, const QString &audio_name, const QByteArray &audio_data, const QString &time)
{
    QString full_audio_name = QString("%1_%2").arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"), audio_name);

    IDBFS_save_audio(full_audio_name, audio_data, static_cast<int>(audio_data.size()));

    emit audio_received(sender, full_audio_name, time);
}

void client_manager::save_group_file(const int &group_ID, const QString &sender, const QString &file_name, const QByteArray &file_data, const QString &time)
{
    emit saving_file(QString("Saving file: %1").arg(file_name));

    QString full_file_name = QString("%1_%2").arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"), file_name);

    IDBFS_save_file(full_file_name, file_data, static_cast<int>(file_data.size()));

    emit group_file_received(group_ID, sender, full_file_name, time);
}

void client_manager::save_group_audio(const int &group_ID, const QString &sender, const QString &audio_name, const QByteArray &audio_data, const QString &time)
{
    QString full_audio_name = QString("%1_%2").arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"), audio_name);

    IDBFS_save_audio(full_audio_name, audio_data, static_cast<int>(audio_data.size()));

    emit group_audio_received(group_ID, sender, full_audio_name, time);
}

void client_manager::send_audio(const QString &sender, const QString &receiver, const QString &audio_name, const QString &time)
{
    _socket->sendBinaryMessage(_protocol->set_audio_message(sender, receiver, audio_name, time));
}

void client_manager::send_lookup_friend(const QString &ID)
{
    _socket->sendBinaryMessage(_protocol->set_lookup_friend_message(ID));
}

void client_manager::send_create_conversation(const int &conversation_ID, const QString &participant1, const int &participant1_ID, const QString &participant2, const int &participant2_ID)
{
    _socket->sendBinaryMessage(_protocol->set_create_conversation_message(conversation_ID, participant1, participant1_ID, participant2, participant2_ID));
}

void client_manager::send_save_conversation(const int &conversation_ID, const QString &sender, const QString &receiver, const QString &content, const QString &time)
{
    _socket->sendBinaryMessage(_protocol->set_save_message_message(conversation_ID, sender, receiver, content, time));
}

void client_manager::send_sign_up(const QString &phone_number, const QString &first_name, const QString &last_name, const QString &password, const QString &secret_question, const QString &secret_answer)
{
    _my_ID = phone_number;

    _socket->sendBinaryMessage(_protocol->set_sign_up_message(phone_number, first_name, last_name, password, secret_question, secret_answer));
}

void client_manager::send_login_request(const QString &phone_number, const QString &password)
{
    _my_ID = phone_number;

    _socket->sendBinaryMessage(_protocol->set_login_request_message(phone_number, password));
}

void client_manager::send_file(const QString &sender, const QString &receiver, const QString &file_name, const QByteArray &file_data, const QString &time)
{
    _socket->sendBinaryMessage(_protocol->set_file_message(sender, receiver, file_name, file_data, time));
}

void client_manager::send_delete_message(const int &conversation_ID, const QString &sender, const QString &receiver, const QString &time)
{
    _socket->sendBinaryMessage(_protocol->set_delete_message(conversation_ID, sender, receiver, time));
}

void client_manager::send_create_new_group(const QString &adm, const QStringList &members, const QString &group_name)
{
    _socket->sendBinaryMessage(_protocol->set_new_group_message(adm, members, group_name));
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

void client_manager::IDBFS_save_audio(const QString &audio_name, const QByteArray &audio_data, const int &size)
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

void client_manager::IDBFS_save_file(const QString &file_name, const QByteArray &file_data, const int &size)
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

void client_manager::delete_audio_IDBFS(const QString &audio_name)
{
    std::string audio_path = "/audio/";
    audio_path += audio_name.toStdString();

    EM_ASM(
        {
            var filePath = Pointer_stringify($0);
            try
            {
                FS.unlink(audio_path);
                console.log('Audio removed from virtual file system');
            }
            catch (e)
            {
                console.error("Failed to delete audio:", e);
            }

            FS.syncfs(false, function(err) {
            assert(!err);
            console.log('Audio system synced with IndexedDB'); });
        },
        audio_path.c_str());
}

void client_manager::delete_file_IDBFS(const QString &file_name)
{
    std::string file_path = "/file/";
    file_path += file_name.toStdString();

    EM_ASM(
        {
            var filePath = Pointer_stringify($0);
            try
            {
                FS.unlink(filePath);
                console.log('File removed from virtual file system');
            }
            catch (e)
            {
                console.error("Failed to delete file:", e);
            }

            FS.syncfs(false, function(err) {
            assert(!err);
            console.log('File system synced with IndexedDB'); });
        },
        file_path.c_str());
}

const QString &client_manager::my_ID() const
{
    return _my_ID;
}

const QString &client_manager::my_name() const
{
    return _my_name;
}

void client_manager::send_group_is_typing(const int &group_ID, const QString &sender)
{
    _socket->sendBinaryMessage(_protocol->set_group_is_typing(group_ID, sender));
}

void client_manager::send_group_text(const int &group_ID, const QString &sender, const QString &message, const QString &time)
{
    _socket->sendBinaryMessage(_protocol->set_group_text_message(group_ID, sender, message, time));
}

void client_manager::send_group_file(const int &group_ID, const QString &sender, const QString &file_name, const QByteArray &file_data, const QString &time)
{
    _socket->sendBinaryMessage(_protocol->set_group_file_message(group_ID, sender, file_name, file_data, time));
}

void client_manager::send_group_audio(const int &group_ID, const QString &sender, const QString &audio_name, const QString &time)
{
    _socket->sendBinaryMessage(_protocol->set_group_audio_message(group_ID, sender, audio_name, time));
}
