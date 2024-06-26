#include "client_main_window.h"

QHash<QString, QWidget *> client_main_window::_window_map = QHash<QString, QWidget *>();

client_chat_window *client_main_window::_server_wid = nullptr;

QHash<QString, std::function<void()>> client_main_window::_settings_choices = QHash<QString, std::function<void()>>();

client_main_window::client_main_window(QWidget *parent)
    : QMainWindow(parent)
{
    _stack = new QStackedWidget(this);

    setCentralWidget(_stack);
    setFixedSize(400, 500);

    _status_bar = new QStatusBar(this);
    setStatusBar(_status_bar);

    connect(this, &client_main_window::swipe_right, this, &client_main_window::on_swipe_right);

    QFile style_file(":/images/style.css");
    if (style_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString style_sheet = QString::fromUtf8(style_file.readAll());
        setStyleSheet(style_sheet);

        style_file.close();
    }

    /*-----------------------------------¬------------------------------------------------------------------------------------------------------------------------------------*/

    QWidget *login_widget = new QWidget();

    _user_phone_number = new QLineEdit(this);

    _user_password = new QLineEdit(this);
    _user_password->setEchoMode(QLineEdit::Password);

    QFormLayout *login = new QFormLayout(this);
    login->addRow("Enter Your Phone Number", _user_phone_number);
    login->addRow("Enter your Password", _user_password);

    _login_button = new QPushButton("Log In", this);
    _login_button->setStyleSheet("background-color: #0077CC;"
                                 "color: white;"
                                 "border: 1px solid #0055AA;"
                                 "border-radius: 5px;"
                                 "padding: 5px 10px;");
    connect(_login_button, &QPushButton::clicked, this, &client_main_window::login);

    QVBoxLayout *VBOX = new QVBoxLayout();
    VBOX->addLayout(login);
    VBOX->addWidget(_login_button);

    QGroupBox *group_box = new QGroupBox();
    group_box->setLayout(VBOX);

    QPushButton *sign_up = new QPushButton("Sign Up", this);
    sign_up->setStyleSheet("background-color: #0077CC;"
                           "color: white;"
                           "border: 1px solid #0055AA;"
                           "border-radius: 5px;"
                           "padding: 5px 10px;");
    connect(sign_up, &QPushButton::clicked, this, [=]()
            { _stack->setCurrentIndex(1); });

    QGridLayout *grid = new QGridLayout(login_widget);
    grid->addWidget(group_box, 0, 0, 1, 1, Qt::AlignCenter);
    grid->addWidget(sign_up, 1, 0, 1, 1, Qt::AlignBottom | Qt::AlignLeft);

    /*-----------------------------------¬------------------------------------------------------------------------------------------------------------------------------------*/

    QWidget *sign_up_widget = new QWidget();
    sign_up_widget->setWindowIconText("Sign Up");

    _insert_first_name = new QLineEdit(this);
    _insert_last_name = new QLineEdit(this);

    _insert_phone_number = new QLineEdit(this);

    _insert_password = new QLineEdit(this);
    _insert_password->setEchoMode(QLineEdit::Password);

    _insert_password_confirmation = new QLineEdit(this);
    _insert_password_confirmation->setEchoMode(QLineEdit::Password);

    _insert_secret_question = new QLineEdit(this);
    _insert_secret_answer = new QLineEdit(this);

    QFormLayout *signup = new QFormLayout(this);
    signup->addRow("First Name", _insert_first_name);
    signup->addRow("Last Name", _insert_last_name);
    signup->addRow("Phone Number", _insert_phone_number);
    signup->addRow("Password", _insert_password);
    signup->addRow("Confirm Password", _insert_password_confirmation);
    signup->addRow("Secret Question", _insert_secret_question);
    signup->addRow("Secret Answer", _insert_secret_answer);

    QPushButton *sign_up_button = new QPushButton("Sign Up", this);
    sign_up_button->setStyleSheet("background-color: #0077CC;"
                                  "color: white;"
                                  "border: 1px solid #0055AA;"
                                  "border-radius: 5px;"
                                  "padding: 5px 10px;");
    connect(sign_up_button, &QPushButton::clicked, this, &client_main_window::sign_up);

    QVBoxLayout *sign_up_layout = new QVBoxLayout();
    sign_up_layout->addLayout(signup);
    sign_up_layout->addWidget(sign_up_button);

    QGroupBox *group_box_2 = new QGroupBox();
    group_box_2->setLayout(sign_up_layout);

    QGridLayout *sign_up_grid = new QGridLayout(sign_up_widget);
    sign_up_grid->addWidget(group_box_2, 0, 0, 1, 1, Qt::AlignCenter);

    /*-----------------------------------¬------------------------------------------------------------------------------------------------------------------------------------*/

    QWidget *chat_widget = new QWidget();

    configure_settings_choice();

    _sidebar = new QFrame(this);
    _sidebar->setFrameShape(QFrame::StyledPanel);
    _sidebar->setFixedWidth(200);
    _sidebar->setStyleSheet("QFrame:hover { background-color: #e0e0e0; }");

    QListWidget *settings = new QListWidget(_sidebar);
    settings->addItem("Chat with an Agent");
    settings->addItem("Change Name");
    settings->addItem("Create New Group");
    settings->addItem("Add People via Phone Number");
    settings->addItem("DELETE ACCOUNT");
    settings->setStyleSheet(R"(
                            QListWidget {
                                border: 2px solid #4A90E2;
                                color: black;
                                font-size: 10px;
                                border-radius: 10px;
                                padding: 5px;
                            }
                            QListWidget::item {
                                border: none;
                                background-color: #EAF4FE;
                                padding: 5px;
                            }
                            QListWidget::item:selected {
                                background-color: #4A90E2;
                                color: white;
                            }
                            )");

    QVBoxLayout *sidebar_layout = new QVBoxLayout(_sidebar);
    sidebar_layout->addWidget(settings);
    _sidebar->hide();

    connect(settings, &QListWidget::itemClicked, this, [=](QListWidgetItem *item)
            {
                if(_settings_choices.contains(item->text()))
                    _settings_choices.value(item->text())();
                else
                    _status_bar->showMessage(QString("Choose 1 Option at a time and not 2"), 5000); });

    QPushButton *toggle_button = new QPushButton("...", this);
    toggle_button->setFixedSize(50, 20);
    connect(toggle_button, &QPushButton::clicked, this, [=]()
            { _sidebar->setVisible(!_sidebar->isVisible()); });

    _friend_list = new QComboBox(this);
    connect(_friend_list, &QComboBox::textActivated, this, &client_main_window::new_conversation);

    _friend_dialog = new QDialog(this);
    _friend_dialog->resize(150, 150);
    _friend_dialog->setWindowTitle("Friend List...");
    _friend_dialog->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout *layout = new QVBoxLayout(_friend_dialog);
    layout->addWidget(_friend_list);

    QPixmap friend_icon(":/images/chat_icon.png");

    QPushButton *friend_button = new QPushButton(this);
    friend_button->setIcon(friend_icon);
    friend_button->setIconSize(QSize(50, 50));
    friend_button->setFixedSize(50, 50);
    friend_button->setStyleSheet("border: none");
    connect(friend_button, &QPushButton::clicked, this, [=]()
            {   client_chat_window::set_window_blur(this, true); 
                _friend_dialog->open();
                connect(_friend_dialog, &QDialog::finished, this, [=]()
                {   client_chat_window::set_window_blur(this, false); }); });

    _group_list = new QComboBox(this);
    connect(_group_list, &QComboBox::textActivated, this, &client_main_window::new_conversation);

    _group_dialog = new QDialog(this);
    _group_dialog->resize(150, 150);
    _group_dialog->setWindowTitle("Group List...");
    _group_dialog->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout *layout_2 = new QVBoxLayout(_group_dialog);
    layout_2->addWidget(_group_list);

    QPixmap groups_icon(":/images/group_icon.png");

    QPushButton *groups = new QPushButton(this);
    groups->setIcon(groups_icon);
    groups->setIconSize(QSize(50, 50));
    groups->setFixedSize(50, 50);
    groups->setStyleSheet("border: none");
    connect(groups, &QPushButton::clicked, this, [=]()
            {   client_chat_window::set_window_blur(this, true);
                _group_dialog->open();
                connect(_group_dialog, &QDialog::finished, this, [=]()
                        { client_chat_window::set_window_blur(this, false); }); });

    QLabel *chats_label = new QLabel("CHATS", this);

    _model = new ChatModel(this, this);

    _list_view = new QListView(this);
    _list_view->setModel(_model);
    _list_view->setItemDelegate(new ChatDelegate(this));
    _list_view->setSelectionMode(QAbstractItemView::NoSelection);
    _list_view->setMinimumWidth(200);
    _list_view->setFont(QFont("Arial", 20));
    connect(_list_view, &QListView::clicked, this, [=](const QModelIndex &index)
            { on_client_name_clicked(index.data(ChatModel::ClientNameRole).toString()); });

    QHBoxLayout *hbox_3 = new QHBoxLayout();
    hbox_3->addWidget(friend_button);
    hbox_3->addWidget(groups);
    hbox_3->addWidget(toggle_button);

    DisplayWidget *display_widget = new DisplayWidget(this);
    display_widget->hide();

    QVBoxLayout *VBOX_2 = new QVBoxLayout();
    VBOX_2->addWidget(display_widget);
    VBOX_2->addLayout(hbox_3);

    VBOX_2->addWidget(chats_label);
    VBOX_2->addWidget(_list_view);

    QHBoxLayout *HBOX = new QHBoxLayout(chat_widget);
    HBOX->addLayout(VBOX_2);
    HBOX->addWidget(_sidebar);

    /*-----------------------------------¬------------------------------------------------------------------------------------------------------------------------------------*/

    _stack->addWidget(login_widget);
    _stack->addWidget(sign_up_widget);
    _stack->addWidget(chat_widget);
}

client_main_window::~client_main_window()
{
    _window_map = QHash<QString, QWidget *>();
}

/*-------------------------------------------------------------------- Slots --------------------------------------------------------------*/

void client_main_window::sign_up()
{
    if (_insert_phone_number->text().toInt() == 0)
    {
        _insert_phone_number->setStyleSheet("border: 1px solid red;");

        _status_bar->showMessage(QString("The Inserted Phone Number is Invalid, Verify it and try again"), 5000);

        return;
    }

    _insert_phone_number->setStyleSheet("border: 1px solid gray;");
    if (_insert_password->text().isEmpty())
    {
        _insert_password->setStyleSheet("border: 1px solid red;");

        _status_bar->showMessage(QString("The Password can't be left empty, Verify it and try again"), 5000);

        return;
    }

    _insert_password->setStyleSheet("border: 1px solid gray;");

    if (_insert_password->text().compare(_insert_password_confirmation->text()))
    {
        _insert_password_confirmation->setStyleSheet("border: 1px solid red;");

        _status_bar->showMessage(QString("The Password Confirmation is incorrect, Verify it and try again"), 5000);

        return;
    }

    _insert_password_confirmation->setStyleSheet("border: 1px solid gray;");

    if (_insert_secret_question->text().isEmpty())
    {
        _insert_secret_question->setStyleSheet("border: 1px solid red;");

        _status_bar->showMessage(QString("The Secret Question can't be left Empty, Verify it and try again"), 5000);

        return;
    }

    _insert_secret_question->setStyleSheet("border: 1px solid gray;");

    if (_insert_secret_answer->text().isEmpty())
    {
        _insert_secret_question->setStyleSheet("border: 1px solid red;");

        _status_bar->showMessage(QString("The Secret Answer can't be left Empty, Verify it and try again"), 5000);

        return;
    }

    _insert_secret_question->setStyleSheet("border: 1px solid gray;");

    QStringList info;
    info << QString("First Name:  %1").arg(_insert_first_name->text())
         << QString(" Last Name: %1").arg(_insert_last_name->text())
         << QString("Phone Number : %1").arg(_insert_phone_number->text())
         << QString("Secret Question : %1").arg(_insert_secret_question->text())
         << QString("Secret Answer : %1").arg(_insert_secret_answer->text());

    ListDialog *input_dialog = new ListDialog(info, "Information Review", this);
    input_dialog->setFixedSize(300, 400);

    connect(input_dialog, &QDialog::accepted, this, [=]()
            {
                if (!_server_wid)
                {
                    _server_wid = new client_chat_window(this);
                    QTimer::singleShot(2000, this, [=]()
                                       { _server_wid->_client->send_sign_up(_insert_phone_number->text(), _insert_first_name->text(), _insert_last_name->text(), _insert_password->text(), _insert_secret_question->text(), _insert_secret_answer->text()); });

                    _status_bar->showMessage(QString("Account Created Successfully, Close the page and try to Log In"), 10000);
                }
                _stack->setCurrentIndex(1);
                

                input_dialog->deleteLater(); });

    input_dialog->open();
}

void client_main_window::login()
{
    _login_button->setDisabled(true);

    if (!_server_wid)
        _server_wid = new client_chat_window(this);

    QTimer::singleShot(2000, this, [=]()
                       { _server_wid->_client->send_login_request(_user_phone_number->text(), _user_password->text()); });

    _status_bar->showMessage("LOADING YOUR DATA, WAIT!!!!!! ...", 30000);

    connect(_server_wid, &client_chat_window::login_request, this, &client_main_window::on_login_request);

    QTimer::singleShot(10000, this, [=]()
                       { _login_button->setEnabled(true); });
}

void client_main_window::on_login_request(const QString &hashed_password, bool true_or_false, const QHash<int, QHash<QString, QString>> &friend_lists, const QStringList &online_friends, const QHash<int, QStringList> &messages, const QHash<int, QHash<int, QString>> &group_lists, const QHash<int, QStringList> &group_messages, const QHash<int, QStringList> &groups_members)
{
    if (hashed_password.isEmpty())
    {
        _user_phone_number->setStyleSheet("border: 1px solid red");

        _status_bar->showMessage(QString("The entered Phone Number is not registered in our System, Verify it and try again"), 5000);

        return;
    }

    _user_phone_number->setStyleSheet("border: 1px solid gray");

    if (true_or_false)
    {
        _stack->setCurrentIndex(2);

        QWidget *wid = _window_map.value("Server");
        if (!wid)
        {
            _stack->addWidget(_server_wid);

            _window_map.insert("Server", _server_wid);

            _status_bar->showMessage("Connected to the Server", 5000);
        }

        connect(_server_wid, &client_chat_window::client_name_changed, this, &client_main_window::on_client_name_changed);
        connect(_server_wid, &client_chat_window::text_message_received, this, &client_main_window::on_text_message_received);
        connect(_server_wid, &client_chat_window::swipe_right, this, &client_main_window::on_swipe_right);
        connect(_server_wid, &client_chat_window::client_added_you, this, &client_main_window::on_client_added_you);
        connect(_server_wid, &client_chat_window::lookup_friend_result, this, &client_main_window::on_lookup_friend_result);
        connect(_server_wid, &client_chat_window::client_disconnected, this, &client_main_window::on_client_disconnected);
        connect(_server_wid, &client_chat_window::client_connected, this, &client_main_window::on_client_connected);
        connect(_server_wid, &client_chat_window::audio_received, this, &client_main_window::on_audio_received);
        connect(_server_wid, &client_chat_window::file_received, this, &client_main_window::on_file_received);
        connect(_server_wid, &client_chat_window::delete_message, this, &client_main_window::on_delete_message);
        connect(_server_wid, &client_chat_window::added_to_group, this, &client_main_window::on_added_to_group);
        connect(_server_wid, &client_chat_window::group_is_typing_received, this, &client_main_window::on_group_is_typing_received);
        connect(_server_wid, &client_chat_window::group_text_received, this, &client_main_window::on_group_text_received);
        connect(_server_wid, &client_chat_window::group_file_received, this, &client_main_window::on_group_file_received);
        connect(_server_wid, &client_chat_window::group_audio_received, this, &client_main_window::on_group_audio_received);
        connect(_server_wid, &client_chat_window::removed_from_group, this, &client_main_window::on_removed_from_group);

        connect(_server_wid, &client_chat_window::new_group_ID, this, [=](const int &group_ID)
                { configure_group(group_ID, _group_name, _group_members, _server_wid->my_name()); });

        connect(_server_wid, &client_chat_window::is_typing_received, this, [=](const QString &sender)
                { _status_bar->showMessage(QString("%1 is typing...").arg(sender), 1000); });

        connect(_server_wid, &client_chat_window::socket_disconnected, this, [=]()
                { _stack->setDisabled(true); _status_bar->showMessage("SERVER DISCONNECTED YOU", 999999); });

        connect(_server_wid, &client_chat_window::data_sent, this, [=](const QString &first_name, const QString &last_message, const int &unread_messages)
                { _model->add_on_top(first_name, last_message, unread_messages); });

        QIcon offline_icon = create_dot_icon(Qt::red, 10);
        QIcon online_icon = create_dot_icon(Qt::green, 10);

        if (!friend_lists.isEmpty())
        {
            for (const int &conversation_ID : friend_lists.keys())
            {
                const QHash<QString, QString> &friend_info = friend_lists[conversation_ID];

                const QStringList &message = messages[conversation_ID];

                for (const QString &name : friend_info.values())
                {
                    _friend_list->addItem(name);
                    _friend_list->setItemData(_friend_list->count() - 1, friend_info.key(name));

                    QIcon valid_icon = online_friends.contains(name) ? online_icon : offline_icon;

                    _friend_list->setItemIcon(_friend_list->count() - 1, valid_icon);

                    if (_window_map.contains(name))
                        continue;

                    client_chat_window *wid = new client_chat_window(conversation_ID, friend_info.key(name), name, this);
                    wid->retrieve_conversation(message);

                    connect(wid, &client_chat_window::swipe_right, this, &client_main_window::on_swipe_right);
                    connect(wid, &client_chat_window::data_sent, this, [=](const QString &first_name, const QString &last_message, const int &unread_messages)
                            { _model->add_on_top(first_name, last_message, unread_messages); });

                    wid->window_name(name);

                    _window_map.insert(name, wid);

                    _stack->addWidget(wid);

                    if (!message.isEmpty())
                        _model->add_chat(name, wid->_last_message, wid->_unread_messages, valid_icon);
                }
            }
        }

        _user_phone_number->clear();
        _user_password->clear();

        if (!group_lists.isEmpty())
        {
            for (const int &group_ID : group_lists.keys())
            {
                const QHash<int, QString> &group_name_and_adm = group_lists[group_ID];

                const QStringList &group_message = group_messages[group_ID];

                const QStringList &group_members = groups_members[group_ID];

                _group_list->addItem(group_name_and_adm.values().first());

                if (_window_map.contains(group_name_and_adm.values().first()))
                    continue;

                QStringList names = authenticate_group_members(group_members);

                client_chat_window *win = new client_chat_window(group_ID, group_name_and_adm.values().first(), names, QString::number(group_name_and_adm.keys().first()), this);
                win->retrieve_group_conversation(group_message);

                connect(win, &client_chat_window::swipe_right, this, &client_main_window::on_swipe_right);
                connect(win, &client_chat_window::data_sent, this, [=](const QString &first_name, const QString &last_message, const int &unread_messages)
                        { _model->add_on_top(first_name, last_message, unread_messages); });

                connect(win, &client_chat_window::item_clicked, this, [=](const QString &name)
                        {
                            int index = _friend_list->findText(name, Qt::MatchExactly);
                            if (index != -1)
                                new_conversation(name);
                            else
                            {
                                _search_phone_number = name;
                                win->_client->send_lookup_friend(name);
                            } });

                win->window_name(group_name_and_adm.values().first());

                _window_map.insert(group_name_and_adm.values().first(), win);

                _stack->addWidget(win);

                if (!group_message.isEmpty())
                    _model->add_chat(group_name_and_adm.values().first(), win->_last_message, win->_unread_messages);
            }
        }
    }
    else
    {
        _user_password->setStyleSheet("border: 1px solid red");

        _status_bar->showMessage(QString("The entered password is incorrect, Verify it and try again"), 5000);

        return;
    }

    _user_password->setStyleSheet("border: 1px solid gray");
}

void client_main_window::configure_settings_choice()
{
    _settings_choices["Chat with an Agent"] = [=]
    {
        QWidget *wid = _window_map.value("Server", this);
        if (wid)
            _stack->setCurrentIndex(_stack->indexOf(wid));
    };

    _settings_choices["Change Name"] = [=]()
    {
        ListDialog *new_name = new ListDialog(QStringList("Enter Desired New Name:"), "Change Name", this, true);

        client_chat_window::set_window_blur(this, true);

        connect(new_name, &QDialog::finished, this, [=]()
                {
                    name_changed(new_name->value_entered());
                    new_name->deleteLater(); });

        connect(new_name, &QDialog::finished, this, [=]()
                { client_chat_window::set_window_blur(this, false); });

        new_name->open();
    };

    _settings_choices["Create New Group"] = [=]()
    { create_group(); };

    _settings_choices["Add People via Phone Number"] = [=]()
    {
        ListDialog *new_friend = new ListDialog(QStringList("Enter The Phone Number"), "New Friend", this, true);

        client_chat_window::set_window_blur(this, true);

        connect(new_friend, &QDialog::accepted, this, [=]()
                {
                    _server_wid->_client->send_lookup_friend(new_friend->value_entered());
                    _search_phone_number = new_friend->value_entered();

                    new_friend->deleteLater(); });

        connect(new_friend, &QDialog::finished, this, [=]()
                { client_chat_window::set_window_blur(this, false); });

        new_friend->open();
    };

    _settings_choices["DELETE ACCOUNT"] = [=]()
    {
        ListDialog *delete_account = new ListDialog(QStringList("Are You Sure You wanna Delete the Account ?"), "Account Deletion", this);

        client_chat_window::set_window_blur(this, true);

        connect(delete_account, &QDialog::accepted, this, [=]()
                { _server_wid->delete_account(); delete_account->deleteLater(); });

        connect(delete_account, &QDialog::finished, this, [=]()
                { client_chat_window::set_window_blur(this, false); });

        delete_account->open();
    };
}

void client_main_window::on_settings()
{
    QStringList choices;
    choices << "Chat with an Agent"
            << "Change Name"
            << "Create New Group"
            << "Add People via Phone Number"
            << "DELETE ACCOUNT";

    ListDialog *settings_dialog = new ListDialog(choices, "Settings", this);

    client_chat_window::set_window_blur(this, true);

    connect(settings_dialog, &QDialog::accepted, this, [=]()
            {
                QString choice = settings_dialog->name_selected().first();

                if(_settings_choices.contains(choice))
                {
                    client_chat_window::set_window_blur(this, false);
                    _settings_choices.value(choice)();
                }
                else
                    _status_bar->showMessage(QString("Choose 1 Option at a time and not 2"), 5000);

                    settings_dialog->deleteLater(); });

    connect(settings_dialog, &QDialog::finished, this, [=]()
            { client_chat_window::set_window_blur(this, false); });

    settings_dialog->open();
}

void client_main_window::name_changed(const QString &name)
{
    if (!name.isEmpty())
    {
        for (QWidget *win : _window_map)
        {
            client_chat_window *wid = qobject_cast<client_chat_window *>(win);
            wid->set_name(name);
        }
    }
}

QIcon client_main_window::create_dot_icon(const QColor &color, int size)
{
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(color);

    int circle_size = size - 4;
    int x = (size - circle_size) / 2;
    int y = (size - circle_size) / 2;
    painter.drawEllipse(x, y, circle_size, circle_size);

    return QIcon(pixmap);
}

void client_main_window::on_client_disconnected(const QString &client_name)
{
    QIcon offline_icon = create_dot_icon(Qt::red, 10);

    set_icon(offline_icon, client_name);

    _status_bar->showMessage(QString("%1 is Disconnected").arg(client_name), 5000);
}

void client_main_window::set_icon(const QIcon &icon, const QString &client_name)
{
    QWidget *win = _window_map.value(client_name);
    if (win)
    {
        int index = _friend_list->findText(client_name);
        if (index != -1)
            _friend_list->setItemIcon(index, icon);

        _model->update_online_icon(client_name, icon);
    }
}

void client_main_window::on_client_connected(const QString &client_name)
{
    QIcon online_icon = create_dot_icon(Qt::green, 10);

    set_icon(online_icon, client_name);

    _status_bar->showMessage(QString("%1 is Connected").arg(client_name), 5000);
}

void client_main_window::on_text_message_received(const QString &sender, const QString &text, const QString &time)
{
    QWidget *win = _window_map.value(sender);
    if (win)
    {
        if (sender.compare("Server"))
        {
            int index = _friend_list->findText(sender, Qt::MatchExactly);
            if (index == -1)
                _friend_list->addItem(sender);
        }

        client_chat_window *wid = qobject_cast<client_chat_window *>(win);
        if (wid)
        {
            wid->text_message_background(text, time);

            if (_active_conversation.compare(sender))
            {
                _model->add_on_top(sender, text, 1);
                wid->_unread_messages++;
            }
            else
                _model->add_on_top(sender, text);
        }
    }
}

void client_main_window::on_client_name_clicked(const QString &client_name)
{
    QWidget *wid = _window_map.value(client_name, this);
    if (wid)
    {
        _stack->setCurrentIndex(_stack->indexOf(wid));
        _model->update_unread_messages(client_name, 0);

        client_chat_window *win = qobject_cast<client_chat_window *>(wid);
        if (win)
            win->in_chat();

        _active_conversation = client_name;
    }
}

void client_main_window::on_client_name_changed(const QString &old_name, const QString &client_name)
{
    QWidget *win = _window_map.value(old_name);
    if (win)
    {
        _window_map.remove(old_name);
        _window_map.insert(client_name, win);

        _model->update_client_name(old_name, client_name);

        int index = _friend_list->findText(old_name);
        if (index != -1)
        {
            _friend_list->removeItem(index);

            _friend_list->insertItem(index, client_name);
        }

        client_chat_window *wind = qobject_cast<client_chat_window *>(win);
        if (wind)
            wind->window_name(client_name);
    }

    QStringList group_names;
    for (int i = 0; i < _group_list->count(); i++)
        group_names << _group_list->itemText(i);

    for (QString name : group_names)
    {
        win = _window_map.value(name);
        if (win)
        {
            client_chat_window *wind = qobject_cast<client_chat_window *>(win);
            if (wind)
            {
                if (wind->_group_members.contains(old_name))
                {
                    wind->_group_members.removeAll(old_name);
                    wind->_group_members << client_name;
                }
            }
        }
    }
}

void client_main_window::on_swipe_right()
{
    if (_stack->currentIndex() > 2)
    {
        _stack->setCurrentIndex(2);
        _active_conversation = QString();
    }
    else
        _stack->setCurrentIndex(0);
}

void client_main_window::on_lookup_friend_result(const int &conversation_ID, const QString &name, bool true_or_false)
{
    if (name.isEmpty())
        return;

    QIcon online_icon = create_dot_icon(Qt::green, 10);
    QIcon offline_icon = create_dot_icon(Qt::red, 10);

    if (_friend_list->findText(name, Qt::MatchExactly) == -1)
    {
        QIcon valid_icon = (true_or_false) ? online_icon : offline_icon;

        _friend_list->addItem(name);

        _friend_list->setItemIcon(_friend_list->count() - 1, valid_icon);
        _friend_list->setItemData(_friend_list->count() - 1, _search_phone_number);

        client_chat_window *wid = new client_chat_window(conversation_ID, _search_phone_number, name, this);
        connect(wid, &client_chat_window::swipe_right, this, &client_main_window::on_swipe_right);
        connect(wid, &client_chat_window::data_sent, this, [=](const QString &first_name, const QString &last_message, const int &unread_messages)
                { _model->add_on_top(first_name, last_message, unread_messages); });

        wid->window_name(name);

        _stack->addWidget(wid);

        _window_map.insert(name, wid);

        _status_bar->showMessage(QString("%1 known as %2 is now in your friend_list").arg(_search_phone_number, name), 5000);
    }
    else
        _status_bar->showMessage(QString("%1 known as %2 is already in your friend_list").arg(_search_phone_number, name), 5000);

    _search_phone_number.clear();
}

void client_main_window::new_conversation(const QString &name)
{
    QWidget *wid = _window_map.value(name, this);
    if (wid)
        _stack->setCurrentIndex(_stack->indexOf(wid));

    _group_dialog->close();
    _friend_dialog->close();
}

void client_main_window::on_client_added_you(const int &conversation_ID, const QString &name, const QString &ID)
{
    if (_friend_list->findText(name, Qt::MatchExactly) == -1)
    {
        _friend_list->addItem(name);

        _friend_list->setItemIcon(_friend_list->count() - 1, create_dot_icon(Qt::green, 10));
        _friend_list->setItemData(_friend_list->count() - 1, ID);

        client_chat_window *wid = new client_chat_window(conversation_ID, ID, name, this);
        if (wid)
        {
            connect(wid, &client_chat_window::swipe_right, this, &client_main_window::on_swipe_right);
            connect(wid, &client_chat_window::data_sent, this, [=](const QString &first_name, const QString &last_message, const int &unread_messages)
                    { _model->add_on_top(first_name, last_message, unread_messages); });

            wid->window_name(name);

            _stack->addWidget(wid);

            _window_map.insert(name, wid);

            _status_bar->showMessage(QString("%1 added You").arg(name), 5000);
        }
    }
}

void client_main_window::on_audio_received(const QString &sender, const QString &audio_name, const QString &time)
{
    QWidget *win = _window_map.value(sender);
    if (win)
    {
        client_chat_window *wid = qobject_cast<client_chat_window *>(win);
        if (wid)
        {
            wid->add_audio(audio_name, false, time);

            if (_active_conversation.compare(sender))
            {
                _model->add_on_top(sender, "voice note", 1);
                wid->_unread_messages++;
            }
            else
                _model->add_on_top(sender, "voice note");
        }
    }
}

void client_main_window::on_file_received(const QString &sender, const QString &file_name, const QString &time)
{
    QWidget *win = _window_map.value(sender);
    if (win)
    {
        client_chat_window *wid = qobject_cast<client_chat_window *>(win);
        if (wid)
        {
            wid->add_file(file_name, false, time);

            if (_active_conversation.compare(sender))
            {
                _model->add_on_top(sender, file_name.split("_").last(), 1);
                wid->_unread_messages++;
            }
            else
                _model->add_on_top(sender, file_name.split("_").last());
        }
    }
}

void client_main_window::on_delete_message(const QString &sender, const QString &time)
{
    QWidget *win = _window_map.value(sender);
    if (win)
    {
        client_chat_window *wid = qobject_cast<client_chat_window *>(win);
        if (wid)
            wid->delete_message_received(time);
    }
}

QStringList client_main_window::authenticate_group_members(const QStringList &group_members_ID)
{
    QStringList names;
    for (QString ID : group_members_ID)
    {
        bool found = false;

        if (!_server_wid->_client->my_ID().compare(ID))
        {
            names << "You";
            continue;
        }

        for (int i = 0; i < _friend_list->count(); i++)
        {
            QString ID_2 = _friend_list->itemData(i).toString();
            if (!ID.compare(ID_2))
            {
                names << _friend_list->itemText(i);
                found = true;

                break;
            }
        }

        if (!found)
            names << ID;
    }

    return names;
}

void client_main_window::configure_group(const int &group_ID, const QString &group_name, const QStringList &names, const QString &adm)
{
    if (_window_map.contains(group_name))
    {
        QWidget *win = _window_map.value(group_name);
        if (win)
        {
            client_chat_window *wid = qobject_cast<client_chat_window *>(win);
            if (wid)
                wid->enable_chat();
        }

        return;
    }

    client_chat_window *win = new client_chat_window(group_ID, group_name, names, adm, this);
    connect(win, &client_chat_window::swipe_right, this, &client_main_window::on_swipe_right);
    connect(win, &client_chat_window::data_sent, this, [=](const QString &first_name, const QString &last_message, const int &unread_messages)
            { _model->add_on_top(first_name, last_message, unread_messages); });

    connect(win, &client_chat_window::item_clicked, this, [=](const QString &name)
            {
                int index = _friend_list->findText(name, Qt::MatchExactly);
                if (index != -1)
                    new_conversation(name);
                else
                {
                    _search_phone_number = name;
                    win->_client->send_lookup_friend(name);
                } });

    win->window_name(group_name);

    _window_map.insert(group_name, win);

    _stack->addWidget(win);

    _group_list->addItem(group_name);
}

void client_main_window::on_added_to_group(const int &group_ID, const QString &adm, const QStringList &group_members, const QString &group_name)
{
    QStringList names = authenticate_group_members(group_members);

    configure_group(group_ID, group_name, names, adm);

    _status_bar->showMessage(QString("%1 Added you do to a Group called: %2").arg(adm, group_name), 5000);
}

void client_main_window::create_group()
{
    ListDialog *input_dialog = new ListDialog(QStringList("Enter Group Name"), "Group Name", this, true);

    client_chat_window::set_window_blur(this, true);

    connect(input_dialog, &QDialog::accepted, this, [=]()
            {   
                _group_name = input_dialog->value_entered();
                if (!_group_name.isEmpty())
                {
                    QStringList friends_name;
                    for (int i = 0; i < _friend_list->count(); i++)
                        friends_name << _friend_list->itemText(i);

                    ListDialog *members = new ListDialog(friends_name, "Select Group Members", this);
                    members->setFixedSize(300, 400);

                    client_chat_window::set_window_blur(this, false);
                    client_chat_window::set_window_blur(this, true);

                    connect(members, &QDialog::accepted, this, [=]()
                    {
                        _group_members = members->name_selected();

                        QStringList IDs;
                        for (QString names : _group_members)
                        {
                            int index = _friend_list->findText(names, Qt::MatchExactly);
                            if (index != -1)
                            {
                                QVariant data = _friend_list->itemData(index);
                                IDs << data.toString();
                            }
                        }

                        _group_members << _server_wid->_client->my_ID();

                        _server_wid->_client->send_create_new_group(_server_wid->my_name(), IDs, _group_name);

                        members->deleteLater();
                    });

                    connect(members, &QDialog::finished, this, [=]()
                    { client_chat_window::set_window_blur(this, false); });

                    members->open();
                }

    input_dialog->deleteLater(); });

    connect(input_dialog, &QDialog::finished, this, [=]()
            { client_chat_window::set_window_blur(this, false); });

    input_dialog->open();
}

void client_main_window::on_group_is_typing_received(const int &group_ID, const QString &group_name, const QString &sender)
{
    QWidget *win = _window_map.value(group_name);
    if (win)
    {
        client_chat_window *wid = qobject_cast<client_chat_window *>(win);
        if (wid)
            _status_bar->showMessage(QString("Group: %1,  %2 is typing...").arg(group_name, sender), 1000);
    }
}

void client_main_window::on_group_text_received(const int &group_ID, const QString &group_name, const QString &sender, const QString &message, const QString &time)
{
    QWidget *win = _window_map.value(group_name);
    if (win)
    {
        int index = _group_list->findText(group_name, Qt::MatchExactly);
        if (index == -1)
            _group_list->addItem(group_name);

        client_chat_window *wid = qobject_cast<client_chat_window *>(win);
        if (wid)
        {
            wid->text_message_background(message, time, sender);

            if (_active_conversation.compare(sender))
            {
                _model->add_on_top(group_name, message, 1);
                wid->_unread_messages++;
            }
            else
                _model->add_on_top(group_name, message);
        }
    }
}

void client_main_window::on_group_audio_received(const int &group_ID, const QString &group_name, const QString &sender, const QString &audio_name, const QString &time)
{
    QWidget *win = _window_map.value(group_name);
    if (win)
    {
        client_chat_window *wid = qobject_cast<client_chat_window *>(win);
        if (wid)
        {
            wid->add_audio(audio_name, false, time, sender);

            if (_active_conversation.compare(sender))
            {
                _model->add_on_top(group_name, "voice note", 1);
                wid->_unread_messages++;
            }
            else
                _model->add_on_top(group_name, "voice note");
        }
    }
}

void client_main_window::on_group_file_received(const int &group_ID, const QString &group_name, const QString &sender, const QString &file_name, const QString &time)
{
    QWidget *win = _window_map.value(group_name);
    if (win)
    {
        client_chat_window *wid = qobject_cast<client_chat_window *>(win);
        if (wid)
        {
            wid->add_file(file_name, false, time, sender);

            if (_active_conversation.compare(sender))
            {
                _model->add_on_top(group_name, file_name.split("_").last(), 1);
                wid->_unread_messages++;
            }
            else
                _model->add_on_top(group_name, file_name.split("_").last());
        }
    }
}

void client_main_window::on_removed_from_group(const int &group_ID, const QString &group_name, const QString &adm)
{
    QWidget *win = _window_map.value(group_name);
    if (win)
    {
        client_chat_window *wid = qobject_cast<client_chat_window *>(win);
        if (wid)
            wid->disable_chat();

        _status_bar->showMessage(QString("You were removed from the Group: %1 by %2").arg(group_name, adm), 5000);
    }
}

/*-------------------------------------------------------------------- Functions --------------------------------------------------------------*/

void client_main_window::mousePressEvent(QMouseEvent *event)
{
    drag_start_position = event->pos();
    dragging = true;
}

void client_main_window::mouseMoveEvent(QMouseEvent *event)
{
    if (dragging && (event->button() != Qt::LeftButton))
    {
        int delta_X = event->pos().x() - drag_start_position.x();

        if (delta_X > 25)
        {
            emit swipe_right();
            dragging = false;
        }
    }
}