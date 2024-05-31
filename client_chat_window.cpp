#include "client_chat_window.h"
#include "client_manager.h"

QString client_chat_window::_my_name = nullptr;
QString client_chat_window::_insert_name = nullptr;

client_manager *client_chat_window::_client = nullptr;

client_chat_window::client_chat_window(QString my_ID, QWidget *parent)
    : QMainWindow(parent), _my_ID(my_ID) { set_up_window(); }

client_chat_window::client_chat_window(int conversation_ID, QString destinator, QString name, QWidget *parent)
    : QMainWindow(parent), _conversation_ID(conversation_ID), _destinator(destinator), _destinator_name(name)
{
    set_up_window();

    ask_microphone_permission();

    QPixmap image_record(":/images/record_icon.png");
    if (!image_record)
        qDebug() << "Image Record Button is NULL";

    QPushButton *record_button = new QPushButton(this);
    record_button->setIcon(image_record);
    record_button->setIconSize(QSize(50, 50));
    record_button->setFixedSize(50, 50);
    record_button->setStyleSheet("border: none");
    connect(record_button, &QPushButton::clicked, this, &client_chat_window::start_recording);

    _duration_label = new QLabel(this);
    _duration_label->hide();
    _hbox->addWidget(record_button);
    _hbox->addWidget(_duration_label);

    _client->send_create_conversation(_conversation_ID, _client->_my_name, _client->_my_ID.toInt(), _destinator_name, _destinator.toInt());

    _send_file_button = new QPushButton("...", this);
    connect(_send_file_button, &QPushButton::clicked, this, &client_chat_window::send_file);

    _hbox->insertWidget(1, _send_file_button);
}

void client_chat_window::message_deleted(QString time)
{
    _client->send_delete_message(_conversation_ID, my_name(), _destinator, time);
}

/*-------------------------------------------------------------------- Slots --------------------------------------------------------------*/

void client_chat_window::on_init_send_file_received(QString sender, QString sender_ID, QString file_name, qint64 file_size)
{
    QString message = QString("-------- %1 --------\n"
                              "%2 wants to send a file. Willing to accept it or not?\n"
                              "File Name: %3\n"
                              "File Size: %4 bytes")
                          .arg(my_name())
                          .arg(sender)
                          .arg(file_name)
                          .arg(QString::number(static_cast<int>(file_size)));

    QMessageBox *message_box = new QMessageBox(this);
    message_box->setWindowTitle("Receiving file");
    message_box->setText("Please review the information below carefully:");
    message_box->setInformativeText(message);
    message_box->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    message_box->setDefaultButton(QMessageBox::Ok);
    connect(message_box, &QMessageBox::accepted, this, [=]()
            { _client->send_file_accepted(my_name(), sender_ID); });

    connect(message_box, &QMessageBox::rejected, this, [=]()
            { _client->send_file_rejected(my_name(), sender_ID); });
}

void client_chat_window::on_file_saved(QString path)
{
    QMessageBox::information(this, "File Saved", QString("File save at: %1").arg(path));

    add_file(path, false, QTime::currentTime().toString());

    emit data_received_sent(_window_name);
}

void client_chat_window::ask_microphone_permission()
{
    QMicrophonePermission microphonePermission;

    switch (qApp->checkPermission(microphonePermission))
    {
    case Qt::PermissionStatus::Undetermined:
        qApp->requestPermission(microphonePermission, this, [=]()
                                { qDebug() << "Undetermined: Microphone permission granted!"; });

        break;

    case Qt::PermissionStatus::Denied:
        qDebug() << "Denied: Microphone permission is Denied!";

        break;

    case Qt::PermissionStatus::Granted:

        _session = new QMediaCaptureSession(this);
        _audio_input = new QAudioInput(this);
        _session->setAudioInput(_audio_input);

        _recorder = new QMediaRecorder(this);
        connect(_recorder, &QMediaRecorder::durationChanged, this, &client_chat_window::on_duration_changed);
        _session->setRecorder(_recorder);

        _recorder->setOutputLocation(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/" + QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") + "_audio.m4a"));
        _recorder->setQuality(QMediaRecorder::VeryHighQuality);
        _recorder->setEncodingMode(QMediaRecorder::ConstantQualityEncoding);

        break;
    }
}

void client_chat_window::start_recording()
{
    QString audio_path = _recorder->outputLocation().toLocalFile();

    if (!is_recording)
    {
        if (QFile::exists(audio_path))
        {
            if (!QFile::remove(audio_path))
            {
                qDebug() << "client_chat_window ---> start_recording() ---> Error: Unable to delete the existing audio file!";
                return;
            }
        }

        _recorder->record();
        _duration_label->show();
        is_recording = true;
    }
    else
    {
        _recorder->stop();
        is_recording = false;
        _duration_label->hide();
        _duration_label->clear();

        QFile audio(audio_path);
        QByteArray audio_data;
        if (audio.open(QIODevice::ReadOnly))
        {
            audio_data = audio.readAll();
            audio.close();
        }
        else
            qDebug() << "client_chat_window ---> start_recording() ---> Failed to open audio for reading:" << audio_path;

        QString audio_name = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") + "_audio.m4a";

        _client->IDBFS_save_audio(audio_name, audio_data, static_cast<int>(audio_data.size()));

        EM_ASM({
            FS.syncfs(function(err) {
                assert(!err);
                console.log('Audio file saved and synced');
            });
        });

        QString current_time = QTime::currentTime().toString();

        add_audio(audio_name, true, current_time);

        _client->send_audio(my_name(), _destinator, audio_path, current_time);

        _client->send_save_audio(_conversation_ID, _client->_my_ID, _destinator, audio_path, "audio", current_time);

        emit data_received_sent(_window_name);
    }
}

void client_chat_window::on_duration_changed(qint64 duration)
{
    if (_recorder->error() != QMediaRecorder::NoError || duration < 1000)
        return;

    QString duration_str = QString("Recording... %1:%2")
                               .arg(duration / 60000, 2, 10, QChar('0'))
                               .arg((duration % 60000) / 1000, 2, 10, QChar('0'));

    _duration_label->setText(duration_str);
}

void client_chat_window::play_audio(const QUrl &source, QPushButton *audio, QSlider *slider)
{
    if (!is_playing)
    {
        slider->show();

        if (paused_position)
        {
            _player->setPosition(paused_position);
            _player->play();

            is_playing = true;

            audio->setText("⏸️");
        }
        else
        {
            _player = new QMediaPlayer(this);
            _audio_output = new QAudioOutput(this);
            _player->setAudioOutput(_audio_output);
            _player->setSource(source);
            _audio_output->setVolume(50);

            connect(slider, &QSlider::valueChanged, _player, &QMediaPlayer::setPosition);

            connect(_player, &QMediaPlayer::durationChanged, this, [=](qint64 duration)
                    {
                        slider->setRange(0, duration);
                        slider->setValue(_player->position()); });

            connect(_player, &QMediaPlayer::playbackStateChanged, this, [=](QMediaPlayer::PlaybackState state)
                    {
                        if (state == QMediaPlayer::StoppedState)
                        {
                            paused_position = 0;

                            slider->hide();

                            is_playing = false;

                            audio->setText("▶️");
                        } });

            QTimer *timer = new QTimer(this);
            connect(timer, &QTimer::timeout, [=]()
                    { slider->setValue(_player->position()); });
            timer->start(100);

            _player->play();

            is_playing = true;

            audio->setText("⏸️");
        }
    }
    else
    {
        paused_position = _player->position();
        _player->pause();

        is_playing = false;

        audio->setText("▶️");
    }
}
/*-------------------------------------------------------------------- Functions --------------------------------------------------------------*/

void client_chat_window::send_message()
{
    QString message = _insert_message->text();

    QString current_time = QTime::currentTime().toString();

    chat_line *wid = new chat_line(this);
    wid->set_message(message, true, current_time);
    wid->setStyleSheet("color: black;");

    QListWidgetItem *line = new QListWidgetItem(_list);
    line->setSizeHint(QSize(0, 60));
    line->setData(Qt::UserRole, current_time);

    line->setBackground(QBrush(QColorConstants::Svg::lightskyblue));

    _list->setItemWidget(line, wid);

    _client->send_text(my_name(), _destinator, message, current_time);

    _insert_message->clear();

    emit data_received_sent(_window_name);
}

void client_chat_window::message_received(QString message, QString time)
{
    chat_line *wid = new chat_line(this);
    wid->set_message(message, false, time);
    wid->setStyleSheet("color: black;");

    QListWidgetItem *line = new QListWidgetItem();
    line->setBackground(QBrush(QColorConstants::Svg::gray));
    line->setSizeHint(QSize(0, 60));
    line->setData(Qt::UserRole, time);

    _list->addItem(line);
    _list->setItemWidget(line, wid);

    _client->send_save_conversation(_conversation_ID, _destinator, _client->_my_ID, message, time);
}

void client_chat_window::delete_message_received(const QString &time)
{
    for (QListWidgetItem *item : _list->findItems(QString("*"), Qt::MatchWildcard))
    {
        if (!item->data(Qt::UserRole).toString().compare(time))
            delete item;
    }
}

void client_chat_window::send_file()
{
    std::function<void(const QString &, const QByteArray &)> file_content_ready = [this](const QString &file_name, const QByteArray &file_data)
    {
        if (!file_name.isEmpty())
        {
            _client->send_init_send_file(my_name(), _client->_my_ID, _destinator, QFileInfo(file_name).fileName(), static_cast<int>(file_data.size()));

            QString IDBFS_file_name = QString("%1_%2").arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"), QFileInfo(file_name).fileName());
            QString current_time = QTime::currentTime().toString();

            connect(_client, &client_manager::file_accepted, this, [=](QString receiver)
                    { add_file(file_name, true, current_time);
                    _client->send_file(my_name(), _destinator, QFileInfo(file_name).fileName(), file_data, current_time);
                    _client->IDBFS_save_file(file_name, file_data, static_cast<int>(file_data.size()));
                    _client->send_save_file(_conversation_ID, _destinator, _client->_my_ID, QFileInfo(file_name).fileName(), file_data, "file", current_time); });
        }
    };

    QFileDialog::getOpenFileContent("All Files (*)", file_content_ready);
}

void client_chat_window::set_up_window()
{
    QWidget *central_widget = new QWidget();
    setCentralWidget(central_widget);

    QPushButton *button_file = new QPushButton("Server's Conversation", this);
    button_file->setStyleSheet("border: none;");
    connect(button_file, &QPushButton::clicked, this, [=]()
            { QDesktopServices::openUrl(QUrl("/file")); });

    connect(this, &client_chat_window::update_button_file, this, [=]()
            { button_file->setText(QString("%1's Conversation").arg(_window_name)); });

    _list = new Swipeable_list_widget(this);
    _list->setItemDelegate(new separator_delegate(_list));

    _insert_message = new QLineEdit(this);
    _insert_message->setPlaceholderText("Insert New Message");
    connect(_insert_message, &QLineEdit::textChanged, this, [=]()
            { _client->send_is_typing(my_name(), _destinator); });

    QPixmap image_send(":/images/send_icon.png");
    if (!image_send)
        qDebug() << "Image Send Button is NULL";

    QPushButton *send_button = new QPushButton(this);
    send_button->setIcon(image_send);
    send_button->setIconSize(QSize(30, 30));
    send_button->setFixedSize(30, 30);
    send_button->setStyleSheet("border: none");
    connect(send_button, &QPushButton::clicked, this, &client_chat_window::send_message);

    _hbox = new QHBoxLayout();
    _hbox->addWidget(_insert_message);
    _hbox->addWidget(send_button);

    QVBoxLayout *VBOX = new QVBoxLayout(central_widget);
    VBOX->addWidget(button_file);
    VBOX->addWidget(_list);
    VBOX->addLayout(_hbox);

    if (!_client)
    {
        _client = new client_manager(this);
        connect(_client, &client_manager::text_message_received, this, [=](QString sender, QString message, QString time)
                { emit text_message_received(sender, message, time); });

        connect(_client, &client_manager::is_typing_received, this, [=](QString sender)
                { emit is_typing_received(sender); });

        connect(_client, &client_manager::client_disconnected, this, [=](QString client_name)
                { emit client_disconnected(client_name); });

        connect(_client, &client_manager::client_connected, this, [=](QString client_name)
                { emit client_connected(client_name); });

        connect(_client, &client_manager::client_name_changed, this, [=](QString old_name, QString client_name)
                { emit client_name_changed(old_name, client_name); });

        connect(_client, &client_manager::socket_disconnected, this, [=]()
                { emit socket_disconnected(); });

        connect(_client, &client_manager::client_added_you, this, [=](int conversation_ID, QString name, QString ID)
                { emit client_added_you(conversation_ID, name, ID); });

        connect(_client, &client_manager::lookup_friend_result, this, [=](int conversation_ID, QString name, bool true_or_false)
                { emit lookup_friend_result(conversation_ID, name, true_or_false); });

        connect(_client, &client_manager::audio_received, this, [=](QString sender, QString audio_name, QString time)
                { emit audio_received(sender, audio_name, time); });

        connect(_client, &client_manager::file_received, this, [=](QString sender, QString file_name, QString time)
                { emit file_received(sender, file_name, time); });

        connect(_client, &client_manager::login_request, this, [=](QString hashed_password, bool true_or_false, QHash<int, QHash<QString, int>> friend_list, QList<QString> online_friends, QHash<int, QVector<QString>> messages, QHash<int, QHash<QString, QByteArray>> binary_data)
                { emit login_request(hashed_password, true_or_false, friend_list, online_friends, messages, binary_data); });

        connect(_client, &client_manager::init_send_file_received, this, &client_chat_window::on_init_send_file_received);

        connect(_client, &client_manager::file_rejected, this, [=](QString sender)
                { QMessageBox *info = new QMessageBox(this); 
                  info->information(this, "File Rejected", QString("%1 has rejected receiving your file").arg(sender)); });

        connect(_client, &client_manager::delete_message, this, [=](const QString &sender, const QString &time)
                { emit delete_message(sender, time); });
    }
}

QString client_chat_window::my_name()
{
    QString name = _insert_name.length() > 0 ? _insert_name : _client->_my_name;

    return name;
}

void client_chat_window::set_name(QString insert_name)
{
    _insert_name = insert_name;

    _client->_my_name = insert_name;

    _client->send_name(insert_name);
}

void client_chat_window::window_name(QString name)
{
    _window_name = name;

    emit update_button_file();
}

void client_chat_window::mousePressEvent(QMouseEvent *event)
{
    _drag_start_position = event->pos();
    _dragging = true;
}

void client_chat_window::mouseMoveEvent(QMouseEvent *event)
{
    if (_dragging && (event->button() != Qt::LeftButton))
    {
        int delta_X = event->pos().x() - _drag_start_position.x();

        if (delta_X > 25)
        {
            emit swipe_right();
            _dragging = false;
        }
    }
}

void client_chat_window::add_file(QString file_name, bool is_mine, QString time)
{
    QWidget *wid = new QWidget();
    wid->setStyleSheet("color: black;");

    QPixmap image(":/images/file_icon.webp");
    if (image.isNull())
        qDebug() << "File Image is NULL";

    QLabel *time_label = new QLabel(time, this);

    QPushButton *file = new QPushButton(this);
    file->setIcon(image);
    file->setIconSize(QSize(30, 30));
    file->setFixedSize(QSize(30, 30));
    file->setStyleSheet("border: none");
    file->connect(file, &QPushButton::clicked, this, [=]()
                  { QDesktopServices::openUrl(_client->get_file_url(file_name)); });

    QHBoxLayout *file_lay = new QHBoxLayout();
    file_lay->addWidget(file);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(file, 7);
    vbox->addWidget(time_label, 3);

    wid->setLayout(vbox);

    QListWidgetItem *line = new QListWidgetItem(_list);
    line->setSizeHint(QSize(0, 68));
    line->setData(Qt::UserRole, time);

    (is_mine) ? line->setBackground(QBrush(QColorConstants::Svg::lightskyblue)) : line->setBackground(QBrush(QColorConstants::Svg::gray));

    _list->setItemWidget(line, wid);
}

void client_chat_window::add_audio(QString audio_name, bool is_mine, QString time)
{
    QWidget *wid = new QWidget();
    wid->setStyleSheet("color: black;");

    QLabel *time_label = new QLabel(time, this);

    QSlider *slider = new QSlider(Qt::Horizontal, this);
    slider->hide();

    QPushButton *audio = new QPushButton("▶️", this);
    connect(audio, &QPushButton::clicked, this, [=]()
            { play_audio(_client->get_audio_url(audio_name), audio, slider); });

    QHBoxLayout *hbox_1 = new QHBoxLayout();
    hbox_1->addWidget(audio);
    hbox_1->addWidget(slider);

    QVBoxLayout *vbox_1 = new QVBoxLayout(wid);
    vbox_1->addLayout(hbox_1, 8);
    vbox_1->addWidget(time_label, 2);

    QListWidgetItem *line = new QListWidgetItem(_list);
    line->setSizeHint(QSize(0, 80));
    line->setData(Qt::UserRole, time);

    (is_mine) ? line->setBackground(QBrush(QColorConstants::Svg::lightskyblue)) : line->setBackground(QBrush(QColorConstants::Svg::gray));

    _list->setItemWidget(line, wid);
}

void client_chat_window::set_retrieve_message_window(QString type, QString content, QByteArray file_data, QString date_time, bool true_or_false)
{
    if (!type.compare("file"))
    {
        QString file_name = QString("%1_%2").arg(date_time, content);

        _client->IDBFS_save_file(file_name, file_data, static_cast<int>(file_data.size()));

        add_file(file_name, true_or_false, date_time);

        return;
    }
    else if (!type.compare("audio"))
    {
        QString audio_name = QString("%1_%2").arg(date_time, content);

        _client->IDBFS_save_audio(audio_name, file_data, static_cast<int>(file_data.size()));

        add_audio(audio_name, true_or_false, date_time);

        return;
    }

    chat_line *wid = new chat_line(this);
    wid->set_message(content, true_or_false, date_time);
    wid->setStyleSheet("color: black;");

    QListWidgetItem *line = new QListWidgetItem(_list);
    line->setSizeHint(QSize(0, 60));
    line->setData(Qt::UserRole, date_time);

    (true_or_false) ? line->setBackground(QBrush(QColorConstants::Svg::lightskyblue)) : line->setBackground(QBrush(QColorConstants::Svg::gray));

    _list->setItemWidget(line, wid);
}

void client_chat_window::retrieve_conversation(QVector<QString> &messages, QHash<QString, QByteArray> &binary_data)
{
    if (messages.isEmpty())
        return;

    for (const QString &message : messages)
    {
        QStringList parts = message.split("/");

        QString sender_ID = parts.first();
        QString receiver_ID = parts.at(1);
        QString content = parts.at(2);
        QString date_time = parts.at(3);
        QString type = parts.last();

        if (!sender_ID.compare(_client->_my_ID))
            set_retrieve_message_window(type, content, binary_data.value(date_time), date_time, true);
        else
            set_retrieve_message_window(type, content, binary_data.value(date_time), date_time, false);
    }

    EM_ASM({
        FS.syncfs(function(err) {
            assert(!err);
            console.log('Audio & File saved and synced');
        });
    });
}

void client_chat_window::add_friend(QString ID)
{
    if (!_client->_my_ID.compare(ID))
        return;
    _client->send_lookup_friend(ID);
}
