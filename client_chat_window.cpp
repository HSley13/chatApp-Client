#include "client_chat_window.h"
#include "client_manager.h"

QString client_chat_window::_my_name = nullptr;
QString client_chat_window::_insert_name = nullptr;

client_manager *client_chat_window::_client = nullptr;

client_chat_window::client_chat_window(QWidget *parent)
    : QMainWindow(parent) { set_up_window(); }

client_chat_window::client_chat_window(const int &conversation_ID, const QString &destinator, const QString &name, QWidget *parent)
    : QMainWindow(parent), _conversation_ID(conversation_ID), _destinator(destinator), _destinator_name(name)
{
    set_up_window();

    ask_microphone_permission();

    set_up_window_2();

    connect(_insert_message, &QLineEdit::textChanged, this, [=]()
            { _client->send_is_typing(my_name(), _destinator); });

    _client->send_create_conversation(_conversation_ID, my_name(), _client->my_ID().toInt(), _destinator_name, _destinator.toInt());
}

client_chat_window::client_chat_window(const int &group_ID, const QString &group_name, const QStringList &group_members, QWidget *parent)
    : QMainWindow(parent), _group_ID(group_ID), _group_name(group_name), _group_members(group_members)
{
    set_up_window();

    ask_microphone_permission();

    set_up_window_2();
}

void client_chat_window::message_deleted(const QString &time)
{
    _client->send_delete_message(_conversation_ID, my_name(), _destinator, time);

    // _client->delete_audio_IDBFS();
    // OR
    // _client->delete_file_IDBFS();
}

/*-------------------------------------------------------------------- Slots --------------------------------------------------------------*/
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

        if (_group_name.isEmpty())
        {
            _client->send_audio(my_name(), _destinator, audio_path, current_time);

            _client->send_save_audio(_conversation_ID, _client->my_ID(), _destinator, audio_path, "audio", current_time);
        }
        else
            _client->send_group_audio(_group_ID, _group_name, my_name(), audio_path, current_time);

        emit data_received_sent(_window_name);
    }
}

void client_chat_window::on_duration_changed(const qint64 &duration)
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

            connect(slider, &QSlider::valueChanged, _player, [=](int position)
                    { _player->setPosition(static_cast<qint64>(position)); });

            connect(_player, &QMediaPlayer::durationChanged, this, [=](qint64 duration)
                    {   slider->setRange(0, static_cast<int>(duration));
                        slider->setValue(static_cast<int>(duration)); });

            connect(_player, &QMediaPlayer::playbackStateChanged, this, [=](QMediaPlayer::PlaybackState state)
                    {
                        if (state == QMediaPlayer::StoppedState)
                        {
                            paused_position = 0;

                            slider->hide();

                            is_playing = false;

                            audio->setText("▶️");
                        } });

            _player->play();

            slider->show();

            is_playing = true;

            audio->setText("⏸️");

            QTimer *timer = new QTimer(this);
            connect(timer, &QTimer::timeout, this, [=]()
                    { slider->setValue(static_cast<int>(_player->position())); });
            timer->start(100);
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

    (_group_name.isEmpty()) ? _client->send_text(my_name(), _destinator, message, current_time) : _client->send_group_text(_group_ID, _group_name, my_name(), message, current_time);

    _insert_message->clear();

    emit data_received_sent(_window_name);
}

void client_chat_window::message_received(const QString &message, const QString &time, const QString &sender)
{
    chat_line *wid = new chat_line(this);

    if (sender.isEmpty())
    {
        wid->set_message(message, false, time);

        if (_destinator.compare("Server"))
            _client->send_save_conversation(_conversation_ID, _destinator, _client->my_ID(), message, time);
    }
    else
        wid->set_group_message(message, sender, false, time);

    wid->setStyleSheet("color: black;");

    QListWidgetItem *line = new QListWidgetItem();
    line->setBackground(QBrush(QColorConstants::Svg::gray));
    line->setSizeHint(QSize(0, 60));
    line->setData(Qt::UserRole, time);

    _list->addItem(line);
    _list->setItemWidget(line, wid);
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
            QString IDBFS_file_name = QString("%1_%2").arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"), QFileInfo(file_name).fileName());
            QString current_time = QTime::currentTime().toString();

            add_file(file_name, true, current_time);

            _client->IDBFS_save_file(file_name, file_data, static_cast<int>(file_data.size()));

            if (_group_name.isEmpty())
            {
                _client->send_file(my_name(), _destinator, QFileInfo(file_name).fileName(), file_data, current_time);
                _client->send_save_file(_conversation_ID, _destinator, _client->my_ID(), QFileInfo(file_name).fileName(), file_data, "file", current_time);
            }
            else
                _client->send_group_file(_group_ID, _group_name, my_name(), QFileInfo(file_name).fileName(), file_data, current_time);
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
            {
                if (_group_name.isEmpty())
                    return;
                else
                {
                    group_member *members = new group_member(_group_members, this);
                    connect(members, &QInputDialog::finished, this, [=](int result)
                            {   
                                if(result == QDialog::Accepted)
                                {
                                    QString name = members->name_selected().first();
                                    emit item_clicked(name);
                                }
                                    members->deleteLater(); });

                    members->open();
                } });

    connect(this, &client_chat_window::update_button_file, this, [=]()
            { button_file->setText(QString("%1's Conversation").arg(_window_name)); });

    _list = new Swipeable_list_widget(this, this);
    _list->setItemDelegate(new separator_delegate(_list));
    _list->setSelectionMode(QAbstractItemView::NoSelection);

    _insert_message = new QLineEdit(this);
    _insert_message->setPlaceholderText("Insert New Message");

    connect(_insert_message, &QLineEdit::textChanged, this, [=]()
            { if(_group_name.isEmpty()) 
                _client->send_is_typing(my_name(), _destinator); 
              else
                 _client->send_group_is_typing(_group_ID, _group_name, my_name()); });

    QPixmap image_send(":/images/send_icon.png");
    if (!image_send)
        qDebug() << "Image Send Button is NULL";

    _send_button = new QPushButton(this);
    _send_button->setIcon(image_send);
    _send_button->setIconSize(QSize(30, 30));
    _send_button->setFixedSize(30, 30);
    _send_button->setStyleSheet("border: none");
    connect(_send_button, &QPushButton::clicked, this, &client_chat_window::send_message);

    _hbox = new QHBoxLayout();
    _hbox->addWidget(_insert_message);
    _hbox->addWidget(_send_button);

    QVBoxLayout *VBOX = new QVBoxLayout(central_widget);
    VBOX->addWidget(button_file);
    VBOX->addWidget(_list);
    VBOX->addLayout(_hbox);

    if (!_client)
    {
        _client = new client_manager(this);
        connect(_client, &client_manager::text_message_received, this, [=](const QString &sender, const QString &message, const QString &time)
                { emit text_message_received(sender, message, time); });

        connect(_client, &client_manager::is_typing_received, this, [=](const QString &sender)
                { emit is_typing_received(sender); });

        connect(_client, &client_manager::client_disconnected, this, [=](const QString &client_name)
                { emit client_disconnected(client_name); });

        connect(_client, &client_manager::client_connected, this, [=](const QString &client_name)
                { emit client_connected(client_name); });

        connect(_client, &client_manager::client_name_changed, this, [=](const QString &old_name, const QString &client_name)
                { emit client_name_changed(old_name, client_name); });

        connect(_client, &client_manager::socket_disconnected, this, [=]()
                { emit socket_disconnected(); });

        connect(_client, &client_manager::client_added_you, this, [=](const int &conversation_ID, const QString &name, const QString &ID)
                { emit client_added_you(conversation_ID, name, ID); });

        connect(_client, &client_manager::lookup_friend_result, this, [=](const int &conversation_ID, const QString &name, bool true_or_false)
                { emit lookup_friend_result(conversation_ID, name, true_or_false); });

        connect(_client, &client_manager::audio_received, this, [=](const QString &sender, const QString &audio_name, const QString &time)
                { emit audio_received(sender, audio_name, time); });

        connect(_client, &client_manager::file_received, this, [=](const QString &sender, const QString &file_name, const QString &time)
                { emit file_received(sender, file_name, time); });

        connect(_client, &client_manager::login_request, this, [=](const QString &hashed_password, bool true_or_false, const QHash<int, QHash<QString, int>> &friend_list, const QList<QString> &online_friends, const QHash<int, QVector<QString>> &messages, const QHash<int, QHash<QString, QByteArray>> &binary_data, const QHash<int, QString> &group_list, const QHash<int, QVector<QString>> &group_messages, const QHash<int, QHash<QString, QByteArray>> &group_binary_data, const QHash<int, QStringList> &groups_members)
                { emit login_request(hashed_password, true_or_false, friend_list, online_friends, messages, binary_data, group_list, group_messages, group_binary_data, groups_members); });

        connect(_client, &client_manager::delete_message, this, [=](const QString &sender, const QString &time)
                { emit delete_message(sender, time); });

        connect(_client, &client_manager::saving_file, this, [=](const QString &message)
                { emit saving_file(message); });

        connect(_client, &client_manager::new_group_ID, this, [=](const int &group_ID)
                { emit new_group_ID(group_ID); });

        connect(_client, &client_manager::added_to_group, this, [=](const int &group_ID, const QString &adm, const QStringList &group_members, const QString &group_name)
                { emit added_to_group(group_ID, adm, group_members, group_name); });

        connect(_client, &client_manager::group_is_typing_received, this, [=](const int &group_ID, const QString &group_name, const QString &sender)
                { emit group_is_typing_received(group_ID, group_name, sender); });

        connect(_client, &client_manager::group_text_received, this, [=](const int &group_ID, const QString &group_name, const QString &sender, const QString &message, const QString time)
                { emit group_text_received(group_ID, group_name, sender, message, time); });

        connect(_client, &client_manager::group_audio_received, this, [=](const int &group_ID, const QString &group_name, const QString &sender, const QString &audio_name, const QString &time)
                { emit group_audio_received(group_ID, group_name, sender, audio_name, time); });

        connect(_client, &client_manager::group_file_received, this, [=](const int &group_ID, const QString &group_name, const QString &sender, const QString &file_name, const QString &time)
                { emit group_file_received(group_ID, group_name, sender, file_name, time); });
    }
}

void client_chat_window::set_up_window_2()
{
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

    _send_file_button = new QPushButton("...", this);
    connect(_send_file_button, &QPushButton::clicked, this, &client_chat_window::send_file);

    _hbox->insertWidget(1, _send_file_button);
}

QString client_chat_window::my_name()
{
    QString name = _insert_name.length() > 0 ? _insert_name : _client->my_name();

    return name;
}

void client_chat_window::set_name(const QString &insert_name)
{
    _insert_name = insert_name;

    _client->send_name(insert_name);
}

void client_chat_window::window_name(const QString &name)
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

void client_chat_window::add_file(const QString &file_name, bool is_mine, const QString &time, const QString &sender)
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

    if (sender.isEmpty())
        file_lay->addWidget(file);
    else
    {
        QLabel *lab = new QLabel(QString("%1: ").arg(sender), this);

        qDebug() << "client_chat_window ---> add_file() ---> inside the condition in which sender is not null";

        file_lay->addWidget(lab, 3);
        file_lay->addWidget(file, 7);
    }

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(file, 7);
    vbox->addWidget(time_label, 3);

    wid->setLayout(vbox);

    QListWidgetItem *line = new QListWidgetItem(_list);
    line->setSizeHint(QSize(0, 68));
    line->setData(Qt::UserRole, time);

    (is_mine) ? line->setBackground(QBrush(QColorConstants::Svg::lightskyblue)) : line->setBackground(QBrush(QColorConstants::Svg::gray));

    _list->setItemWidget(line, wid);

    emit saving_file("file saved");
}

void client_chat_window::add_audio(const QString &audio_name, bool is_mine, const QString &time, const QString &sender)
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

    if (sender.isEmpty())
    {
        hbox_1->addWidget(audio);
        hbox_1->addWidget(slider);
    }
    else
    {
        QLabel *lab = new QLabel(QString("%1: ").arg(sender), this);

        hbox_1->addWidget(lab, 3);
        hbox_1->addWidget(audio, 2);
        hbox_1->addWidget(slider, 5);
    }

    QVBoxLayout *vbox_1 = new QVBoxLayout(wid);
    vbox_1->addLayout(hbox_1, 8);
    vbox_1->addWidget(time_label, 2);

    QListWidgetItem *line = new QListWidgetItem(_list);
    line->setSizeHint(QSize(0, 80));
    line->setData(Qt::UserRole, time);

    (is_mine) ? line->setBackground(QBrush(QColorConstants::Svg::lightskyblue)) : line->setBackground(QBrush(QColorConstants::Svg::gray));

    _list->setItemWidget(line, wid);
}

void client_chat_window::set_retrieve_message_window(const QString &type, const QString &content, const QByteArray &file_data, const QString &date_time, bool true_or_false, const QString &sender)
{
    if (!type.compare("file"))
    {
        QString file_name = QString("%1_%2").arg(date_time, content);

        _client->IDBFS_save_file(file_name, file_data, static_cast<int>(file_data.size()));

        (sender.isEmpty()) ? add_file(file_name, true_or_false, date_time) : add_file(file_name, true_or_false, date_time, sender);

        return;
    }
    else if (!type.compare("audio"))
    {
        QString audio_name = QString("%1_%2").arg(date_time, content);

        _client->IDBFS_save_audio(audio_name, file_data, static_cast<int>(file_data.size()));

        (sender.isEmpty()) ? add_audio(audio_name, true_or_false, date_time) : add_audio(audio_name, true_or_false, date_time, sender);

        return;
    }

    chat_line *wid = new chat_line(this);

    (sender.isEmpty()) ? wid->set_message(content, true_or_false, date_time) : wid->set_group_message(content, sender, true_or_false, date_time);

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

    for (QString message : messages)
    {
        QStringList parts = message.split("/");

        QString sender_ID = parts.first();
        QString receiver_ID = parts.at(1);
        QString content = parts.at(2);
        QString date_time = parts.at(3);
        QString type = parts.last();

        if (!sender_ID.compare(_client->my_ID()))
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

void client_chat_window::retrieve_group_conversation(QVector<QString> &messages, QHash<QString, QByteArray> &binary_data)
{
    if (messages.isEmpty())
        return;

    for (QString message : messages)
    {
        QStringList parts = message.split("/");

        QString sender = parts.first();
        QString content = parts.at(1);
        QString date_time = parts.at(2);
        QString type = parts.last();

        if (!sender.compare(my_name()))
            set_retrieve_message_window(type, content, binary_data.value(date_time), date_time, true);
        else
            set_retrieve_message_window(type, content, binary_data.value(date_time), date_time, false, sender);
    }

    EM_ASM({
        FS.syncfs(function(err) {
            assert(!err);
            console.log('Audio & File saved and synced');
        });
    });
}

void client_chat_window::add_friend(const QString &ID)
{
    if (!_client->my_ID().compare(ID))
        return;
    _client->send_lookup_friend(ID);
}

void client_chat_window::create_new_group(QStringList group_members, QString group_name)
{
    group_members << _client->my_ID();

    _client->send_create_new_group(my_name(), group_members, group_name);
}