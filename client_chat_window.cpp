#include "client_chat_window.h"
#include "client_manager.h"

QString client_chat_window::_my_name = nullptr;
QString client_chat_window::_insert_name = nullptr;

client_manager *client_chat_window::_client = nullptr;
QColor client_chat_window::_last_color;

int client_chat_window::_color_counter = 0;
QString client_chat_window::_last_sender = nullptr;

QColor client_chat_window::_colors[] = {QColorConstants::Svg::lightsalmon, QColorConstants::Svg::lightgreen, QColorConstants::Svg::lightgray, QColorConstants::Svg::lightgoldenrodyellow, QColorConstants::Svg::lightpink};

client_chat_window::client_chat_window(QWidget *parent)
    : QMainWindow(parent) { set_up_window(); }

client_chat_window::client_chat_window(const int &conversation_ID, const QString &destinator, const QString &name, QWidget *parent)
    : QMainWindow(parent), _conversation_ID(conversation_ID), _destinator(destinator), _destinator_name(name)
{
    set_up_window();

    set_up_window_2();

    _client->send_create_conversation(_conversation_ID, my_name(), _client->my_ID().toInt(), _destinator_name, _destinator.toInt());
}

client_chat_window::client_chat_window(const int &group_ID, const QString &group_name, const QStringList &group_members, const QString &adm, QWidget *parent)
    : QMainWindow(parent), _group_ID(group_ID), _group_name(group_name), _group_members(group_members), _adm(adm)
{
    set_up_window();

    if (!_adm.compare(_client->my_ID()))
    {
        QPushButton *settings = new QPushButton("...", this);
        settings->setFixedSize(50, 20);
        connect(settings, &QPushButton::clicked, this, &client_chat_window::on_settings);

        _buttons->addWidget(settings);
    }

    set_up_window_2();
}

void client_chat_window::message_deleted(const QString &time)
{
    delete_message_received(time);

    (_group_name.isEmpty()) ? _client->send_delete_message(_conversation_ID, my_name(), _destinator, time) : _client->send_delete_group_message(_group_ID, _group_name, time);

    _client->send_last_message_read(_conversation_ID, _client->my_ID(), _list->item(_list->count() - 1)->data(Qt::UserRole).toString());
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
        break;

    case Qt::PermissionStatus::Granted:

        _session = new QMediaCaptureSession(this);
        _audio_input = new QAudioInput(this);
        _session->setAudioInput(_audio_input);

        _recorder = new QMediaRecorder(this);
        connect(_recorder, &QMediaRecorder::durationChanged, this, &client_chat_window::on_duration_changed);
        _session->setRecorder(_recorder);

        _recorder->setOutputLocation(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/" + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") + "_audio.m4a"));
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
                return;
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

        QByteArray audio_data;

        QFile audio(audio_path);
        if (audio.open(QIODevice::ReadOnly))
        {
            audio_data = audio.readAll();
            audio.close();
        }

        QString current_time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

        QString audio_name = current_time + "_audio.m4a";

        _client->IDBFS_save_audio(audio_name, audio_data, static_cast<int>(audio_data.size()));

        add_audio(audio_name, true, current_time.split(" ").last());

        if (_group_name.isEmpty())
        {
            _client->send_audio(my_name(), _destinator, audio_name, audio_data, current_time.split(" ").last());

            _client->send_save_data(_conversation_ID, _client->my_ID(), _destinator, "audio.m4a", audio_data, "audio", current_time);
        }
        else
            _client->send_group_audio(_group_ID, _group_name, my_name(), audio_name, audio_data, current_time);

        emit data_sent(_window_name, "voice note", _unread_messages);
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

void client_chat_window::on_settings()
{
    QStringList choices;
    choices << "Add New Member"
            << "Remove Member";

    ListDialog *add_remove_dialog = new ListDialog(choices, "Add/Remove Member", this);
    connect(add_remove_dialog, &QDialog::accepted, this, [=]()
            { 
                                QString option = add_remove_dialog->name_selected().first();

                                if (!option.compare("Add New Member"))
                                {
                                    QInputDialog *add_dialog = new QInputDialog(this);
                                    add_dialog->setWindowTitle("Add New Member");
                                    add_dialog->setLabelText("Enter Phone Number");

                                    connect(add_dialog, &QInputDialog::finished, this, [=](int result)
                                            { 
                                                if(result == QDialog::Accepted)
                                                    _client->send_new_group_member_message(_group_ID, _group_name, my_name(), add_dialog->textValue());

                                                add_dialog->deleteLater(); 
                                            });

                                    add_dialog->open();
                                }
                                else 
                                {
                                    QInputDialog *remove_dialog = new QInputDialog(this);
                                    remove_dialog->setWindowTitle("Remove Member");
                                    remove_dialog->setLabelText("Enter Member Phone Number");

                                    connect(remove_dialog, &QInputDialog::finished, this, [=](int result)
                                            {  
                                                if(result == QDialog::Accepted) 
                                                    _client->send_remove_group_member_message(_group_ID, _group_name, my_name(), remove_dialog->textValue()); 

                                                remove_dialog->deleteLater(); 
                                            });

                                    remove_dialog->open();
                                } 
                                add_remove_dialog->deleteLater(); });

    add_remove_dialog->open();
}

/*-------------------------------------------------------------------- Functions --------------------------------------------------------------*/

void client_chat_window::set_window_blur(QWidget *window, bool blur)
{
    if (blur)
    {
        QGraphicsBlurEffect *blur_effect = new QGraphicsBlurEffect;
        blur_effect->setBlurRadius(10);
        window->setGraphicsEffect(blur_effect);
    }
    else
        window->setGraphicsEffect(nullptr);
}

void client_chat_window::delete_message_received(const QString &time)
{
    for (QListWidgetItem *item : _list->findItems(QString("*"), Qt::MatchWildcard))
    {
        if (!item->data(Qt::UserRole).toString().compare(time))
            delete item;
    }

    (_unread_messages <= 0) ? _unread_messages : _unread_messages--;

    emit data_sent(_window_name, Swipeable_list_widget::item_message(_list->item(_list->count() - 1)), _unread_messages);
}

void client_chat_window::send_message()
{
    QString message = _insert_message->text();

    QString current_time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    chat_line *wid = new chat_line(this);
    wid->setStyleSheet("color: black;");
    wid->set_message(message, true, current_time.split(" ").last());

    QListWidgetItem *line = new QListWidgetItem(_list);
    line->setSizeHint(QSize(0, 60));
    line->setData(Qt::UserRole, current_time.split(" ").last());
    line->setBackground(QBrush(QColorConstants::Svg::lightskyblue));

    line->setSizeHint(wid->sizeHint());
    _list->setItemWidget(line, wid);

    (_group_name.isEmpty()) ? _client->send_text(my_name(), _destinator, message, current_time.split(" ").last()) : _client->send_group_text(_group_ID, _group_name, my_name(), message, current_time);

    _insert_message->clear();

    emit data_sent(_window_name, message, _unread_messages);

    if (_destinator.compare("Server"))
        _client->send_save_conversation(_conversation_ID, _client->my_ID(), _destinator, message, current_time);
}

void client_chat_window::text_message_background(const QString &content, const QString &date_time, const QString &sender, bool true_or_false)
{
    chat_line *wid = new chat_line(this);
    wid->setStyleSheet("color: black;");

    QListWidgetItem *line = new QListWidgetItem(_list);
    line->setSizeHint(QSize(0, 60));
    line->setData(Qt::UserRole, date_time);

    if (sender.isEmpty())
    {
        (true_or_false) ? line->setBackground(QBrush(QColorConstants::Svg::lightskyblue)) : line->setBackground(QBrush(QColorConstants::Svg::lightgray));

        wid->set_message(content, true_or_false, date_time);
    }
    else
    {
        wid->set_group_message(content, sender, true_or_false, date_time);

        if (true_or_false)
            line->setBackground(QBrush(QColorConstants::Svg::lightskyblue));
        else
        {
            if (!_last_sender.compare(sender))
                line->setBackground(QBrush(_last_color));
            else
            {
                line->setBackground(QBrush(_colors[_color_counter % 5]));
                _last_color = _colors[_color_counter % 5];

                _color_counter++;
            }

            _last_sender = sender;
        }
    }

    line->setSizeHint(wid->sizeHint());
    _list->setItemWidget(line, wid);
}

void client_chat_window::send_file()
{
    std::function<void(const QString &, const QByteArray &)> file_content_ready = [=](const QString &file_name, const QByteArray &file_data)
    {
        if (!file_name.isEmpty())
        {
            QString current_time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

            QString IDBFS_file_name = QString("%1_%2").arg(current_time, QFileInfo(file_name).fileName());

            add_file(IDBFS_file_name, true, current_time.split(" ").last());

            emit data_sent(_window_name, QFileInfo(file_name).fileName(), _unread_messages);

            _client->IDBFS_save_file(IDBFS_file_name, file_data, static_cast<int>(file_data.size()));

            if (_group_name.isEmpty())
            {
                _client->send_file(my_name(), _destinator, IDBFS_file_name, file_data, current_time.split(" ").last());
                _client->send_save_data(_conversation_ID, _client->my_ID(), _destinator, QFileInfo(file_name).fileName(), file_data, "file", current_time);
            }
            else
                _client->send_group_file(_group_ID, _group_name, my_name(), IDBFS_file_name, file_data, current_time);
        }
    };

    QFileDialog::getOpenFileContent("All Files (*)", file_content_ready);
}

void client_chat_window::set_up_window()
{
    QWidget *central_widget = new QWidget();
    setCentralWidget(central_widget);

    ask_microphone_permission();

    QPushButton *member_list = new QPushButton("Server's Conversation", this);
    member_list->setStyleSheet("border: none;");
    connect(member_list, &QPushButton::clicked, this, [=]()
            {
                if (_group_name.isEmpty())
                    return;
                else
                {
                    ListDialog *members = new ListDialog(_group_members, "Group Members", this);
                    connect(members, &QDialog::accepted, this, [=]()
                            {   QString name = members->name_selected().first();
                                emit item_clicked(name);

                                members->deleteLater(); });

                    members->open();
                } });

    connect(this, &client_chat_window::update_button_file, this, [=]()
            { member_list->setText(QString("%1's Conversation").arg(_window_name)); });

    _list = new Swipeable_list_widget(this, this);
    _list->setItemDelegate(new separator_delegate(_list));
    _list->setSelectionMode(QAbstractItemView::NoSelection);

    _insert_message = new CustomLineEdit(this);
    _insert_message->setPlaceholderText("Insert New Message");

    connect(_insert_message, &CustomLineEdit::textChanged, this, [=]()
            { (_group_name.isEmpty()) ? _client->send_is_typing(my_name(), _destinator) : _client->send_group_is_typing(_group_ID, _group_name, my_name()); });

    DisplayWidget *display_widget = new DisplayWidget(this);
    display_widget->hide();

    connect(_insert_message, &CustomLineEdit::textChanged, display_widget, &DisplayWidget::setText);
    connect(_insert_message, &CustomLineEdit::focusGained, display_widget, &QWidget::show);
    connect(_insert_message, &CustomLineEdit::focusLost, display_widget, &QWidget::hide);

    QPixmap image_send(":/images/send_icon.png");

    _send_button = new QPushButton(this);
    _send_button->setIcon(image_send);
    _send_button->setIconSize(QSize(30, 30));
    _send_button->setFixedSize(30, 30);
    _send_button->setStyleSheet("border: none");
    connect(_send_button, &QPushButton::clicked, this, &client_chat_window::send_message);

    _hbox = new QHBoxLayout();
    _hbox->addWidget(_insert_message);
    _hbox->addWidget(_send_button);

    _buttons = new QHBoxLayout();
    _buttons->addWidget(member_list);

    QVBoxLayout *VBOX = new QVBoxLayout(central_widget);
    VBOX->addWidget(display_widget);
    VBOX->addLayout(_buttons);
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

        connect(_client, &client_manager::login_request, this, [=](const QString &hashed_password, bool true_or_false, const QHash<int, QHash<QString, QString>> &friend_list, const QStringList &online_friends, const QHash<int, QStringList> &messages, const QHash<int, QHash<int, QString>> &group_list, const QHash<int, QStringList> &group_messages, const QHash<int, QStringList> &groups_members)
                { emit login_request(hashed_password, true_or_false, friend_list, online_friends, messages, group_list, group_messages, groups_members); });

        connect(_client, &client_manager::delete_message, this, [=](const QString &sender, const QString &time)
                { emit delete_message(sender, time); });

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

        connect(_client, &client_manager::removed_from_group, this, [=](const int &group_ID, const QString &group_name, const QString &adm)
                { emit removed_from_group(group_ID, group_name, adm); });
    }
}

void client_chat_window::set_up_window_2()
{
    QPixmap image_record(":/images/record_icon.png");

    _record_button = new QPushButton(this);
    _record_button->setIcon(image_record);
    _record_button->setIconSize(QSize(50, 50));
    _record_button->setFixedSize(50, 50);
    _record_button->setStyleSheet("border: none");
    connect(_record_button, &QPushButton::clicked, this, &client_chat_window::start_recording);

    _duration_label = new QLabel(this);
    _duration_label->hide();
    _hbox->addWidget(_record_button);
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

    QLabel *time_label = new QLabel(time, this);

    const QString &type = (_group_name.isEmpty()) ? "normal" : "group";
    const int &ID = (_group_name.isEmpty()) ? _conversation_ID : _group_ID;

    QPushButton *file = new QPushButton(this);
    file->setIcon(image);
    file->setIconSize(QSize(30, 30));
    file->setFixedSize(QSize(30, 30));
    file->setStyleSheet("border: none");
    connect(file, &QPushButton::clicked, this, [=]()
            { QDesktopServices::openUrl(_client->get_file_url(file_name, ID, type)); });

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(file);
    vbox->addWidget(time_label, 0, Qt::AlignHCenter);

    audio_file_message_background(wid, is_mine, sender, time, vbox);
}

void client_chat_window::add_audio(const QString &audio_name, bool is_mine, const QString &time, const QString &sender)
{
    QWidget *wid = new QWidget();
    wid->setStyleSheet("color: black;");

    QLabel *time_label = new QLabel(time, this);

    QSlider *slider = new QSlider(Qt::Horizontal, this);
    slider->hide();

    const QString &type = (_group_name.isEmpty()) ? "normal" : "group";
    const int &ID = (_group_name.isEmpty()) ? _conversation_ID : _group_ID;

    QPushButton *audio = new QPushButton("▶️", this);
    connect(audio, &QPushButton::clicked, this, [=]()
            { play_audio(_client->get_audio_url(audio_name, ID, type), audio, slider); });

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(audio);
    vbox->addWidget(time_label, 0, Qt::AlignHCenter);

    audio_file_message_background(wid, is_mine, sender, time, vbox, slider);
}

void client_chat_window::audio_file_message_background(QWidget *wid, const bool &is_mine, const QString &sender, const QString &time, QVBoxLayout *vbox, QSlider *slider)
{
    QListWidgetItem *line = new QListWidgetItem(_list);
    line->setSizeHint(QSize(0, 80));
    line->setData(Qt::UserRole, time);

    QHBoxLayout *hbox = new QHBoxLayout(wid);

    if (sender.isEmpty())
    {
        if (is_mine)
        {
            hbox->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

            hbox->addLayout(vbox);

            if (slider != nullptr)
                hbox->addWidget(slider);

            line->setBackground(QBrush(QColorConstants::Svg::lightskyblue));
        }
        else
        {
            line->setBackground(QBrush(QColorConstants::Svg::lightgray));

            hbox->addLayout(vbox);

            if (slider != nullptr)
                hbox->addWidget(slider);

            hbox->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
        }
    }
    else
    {
        QLabel *lab = new QLabel(QString("%1: ").arg(sender), this);
        hbox->addWidget(lab);
        hbox->addLayout(vbox);

        if (slider != nullptr)
            hbox->addWidget(slider);

        hbox->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

        if (is_mine)
            line->setBackground(QBrush(QColorConstants::Svg::lightskyblue));
        else
        {
            if (!_last_sender.compare(sender))
                line->setBackground(QBrush(_last_color));
            else
            {
                line->setBackground(QBrush(_colors[_color_counter % 5]));
                _last_color = _colors[_color_counter % 5];

                _color_counter++;
            }

            _last_sender = sender;
        }
    }

    line->setSizeHint(wid->sizeHint());
    _list->setItemWidget(line, wid);
}

void client_chat_window::set_retrieve_message_window(const QString &type, const QString &content, const QString &date_time, bool true_or_false, const QString &sender)
{
    QString time_display;
    (time_difference(date_time, QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")) >= 24) ? time_display = date_time : time_display = date_time.split(" ").last();

    if (!type.compare("file"))
    {
        QString file_name = QString("%1_%2").arg(date_time, content);

        (sender.isEmpty()) ? add_file(file_name, true_or_false, time_display) : add_file(file_name, true_or_false, time_display, sender);

        return;
    }
    else if (!type.compare("audio"))
    {
        QString audio_name = QString("%1_%2").arg(date_time, content);

        (sender.isEmpty()) ? add_audio(audio_name, true_or_false, time_display) : add_audio(audio_name, true_or_false, time_display, sender);

        return;
    }

    text_message_background(content, time_display, sender, true_or_false);
}

int client_chat_window::time_difference(const QString &date_time1, const QString &date_time2)
{
    QDateTime dt1 = QDateTime::fromString(date_time1, "yyyy-MM-dd HH:mm:ss");
    QDateTime dt2 = QDateTime::fromString(date_time2, "yyyy-MM-dd HH:mm:ss");

    qint64 seconds_difference = dt1.secsTo(dt2);
    int hours_difference = seconds_difference / 3600;

    return hours_difference;
}

void client_chat_window::retrieve_conversation(const QStringList &messages)
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
        QString type = parts.at(4);

        _unread_messages = parts.last().toInt();

        (!sender_ID.compare(_client->my_ID())) ? set_retrieve_message_window(type, content, date_time, true) : set_retrieve_message_window(type, content, date_time, false);
    }

    _last_message = Swipeable_list_widget::item_message(_list->item(_list->count() - 1));
}

void client_chat_window::retrieve_group_conversation(const QStringList &messages)
{
    if (messages.isEmpty())
        return;

    for (QString message : messages)
    {
        QStringList parts = message.split("/");

        QString sender = parts.first();
        QString content = parts.at(1);
        QString date_time = parts.at(2);
        QString type = parts.at(3);

        _unread_messages = parts.last().toInt();

        (!sender.compare(my_name())) ? set_retrieve_message_window(type, content, date_time, true) : set_retrieve_message_window(type, content, date_time, false, sender);
    }

    _last_message = Swipeable_list_widget::item_message(_list->item(_list->count() - 1));
}

void client_chat_window::disable_chat()
{
    _insert_message->setDisabled(true);
    _send_button->setDisabled(true);
    _send_file_button->setDisabled(true);
    _record_button->setDisabled(true);
}

void client_chat_window::enable_chat()
{
    _insert_message->setEnabled(true);
    _send_button->setEnabled(true);
    _send_file_button->setEnabled(true);
    _record_button->setEnabled(true);
}

void client_chat_window::delete_account()
{
    _client->send_delete_account_message(_client->my_ID());

    disable_chat();
}

void client_chat_window::in_chat()
{
    _unread_messages = 0;

    if (_group_name.isEmpty())
        _client->send_last_message_read(_conversation_ID, _client->my_ID(), _list->item(_list->count() - 1)->data(Qt::UserRole).toString());
    else
        _client->send_group_last_message_read(_group_ID, _client->my_ID(), _list->item(_list->count() - 1)->data(Qt::UserRole).toString());
}