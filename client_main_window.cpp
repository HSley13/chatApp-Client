#include "client_main_window.h"

QHash<QString, QWidget *> client_main_window::_window_map = QHash<QString, QWidget *>();

client_chat_window *client_main_window::_server_wid = nullptr;

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
    style_file.open(QFile::ReadOnly);
    QString style_sheet = QLatin1String(style_file.readAll());
    setStyleSheet(style_sheet);

    /*-----------------------------------¬------------------------------------------------------------------------------------------------------------------------------------*/

    QWidget *login_widget = new QWidget();

    QLabel *id_label = new QLabel("Enter Your Phone Number: ", this);
    _user_phone_number = new QLineEdit(this);

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(id_label);
    hbox->addWidget(_user_phone_number);

    QLabel *password_label = new QLabel("Enter your Password: ", this);
    _user_password = new QLineEdit(this);
    _user_password->setEchoMode(QLineEdit::Password);

    QHBoxLayout *hbox_1 = new QHBoxLayout();
    hbox_1->addWidget(password_label);
    hbox_1->addWidget(_user_password);

    QPushButton *log_in = new QPushButton("Log In", this);
    log_in->setStyleSheet("background-color: #0077CC;"
                          "color: white;"
                          "border: 1px solid #0055AA;"
                          "border-radius: 5px;"
                          "padding: 5px 10px;");
    connect(log_in, &QPushButton::clicked, this, [=]()
            {  
                log_in->setDisabled(true);
                   if (!_server_wid)
                _server_wid = new client_chat_window(_user_phone_number->text(), this);
                connect(_server_wid, &client_chat_window::login_request, this, &client_main_window::on_login_request);
                _status_bar->showMessage("LOADING YOUR DATA, WAIT!!!!!! ...", 10000);
                QTimer::singleShot(2000, this, [=]() { _server_wid->_client->send_login_request(_user_phone_number->text(), _user_password->text());});
                QTimer::singleShot(10000, this, [=](){log_in->setEnabled(true); }); });

    QVBoxLayout *VBOX = new QVBoxLayout();
    VBOX->addLayout(hbox);
    VBOX->addLayout(hbox_1);
    VBOX->addWidget(log_in);

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

    QLabel *first_name_label = new QLabel("First Name: ", this);
    _insert_first_name = new QLineEdit(this);
    QHBoxLayout *first_name_layout = new QHBoxLayout();
    first_name_layout->addWidget(first_name_label);
    first_name_layout->addWidget(_insert_first_name);

    QLabel *last_name_label = new QLabel("Last Name: ", this);
    _insert_last_name = new QLineEdit(this);
    QHBoxLayout *last_name_layout = new QHBoxLayout();
    last_name_layout->addWidget(last_name_label);
    last_name_layout->addWidget(_insert_last_name);

    QLabel *phone_number_label = new QLabel("Phone Number: ", this);
    _insert_phone_number = new QLineEdit(this);
    QHBoxLayout *phone_number_layout = new QHBoxLayout();
    phone_number_layout->addWidget(phone_number_label);
    phone_number_layout->addWidget(_insert_phone_number);

    QLabel *password_label_2 = new QLabel("Password: ", this);
    _insert_password = new QLineEdit(this);
    _insert_password->setEchoMode(QLineEdit::Password);
    QHBoxLayout *password_layout = new QHBoxLayout();
    password_layout->addWidget(password_label_2);
    password_layout->addWidget(_insert_password);

    QLabel *password_confirm_label = new QLabel("Confirm Password: ", this);
    _insert_password_confirmation = new QLineEdit(this);
    _insert_password_confirmation->setEchoMode(QLineEdit::Password);
    QHBoxLayout *password_confirm_layout = new QHBoxLayout();
    password_confirm_layout->addWidget(password_confirm_label);
    password_confirm_layout->addWidget(_insert_password_confirmation);

    QLabel *secret_question_label = new QLabel("Secret Question: ", this);
    _insert_secret_question = new QLineEdit(this);
    QHBoxLayout *secret_question_layout = new QHBoxLayout();
    secret_question_layout->addWidget(secret_question_label);
    secret_question_layout->addWidget(_insert_secret_question);

    QLabel *secret_answer_label = new QLabel("Secret Answer: ", this);
    _insert_secret_answer = new QLineEdit(this);
    QHBoxLayout *secret_answer_layout = new QHBoxLayout();
    secret_answer_layout->addWidget(secret_answer_label);
    secret_answer_layout->addWidget(_insert_secret_answer);

    QPushButton *sign_up_button = new QPushButton("Sign Up", this);
    sign_up_button->setStyleSheet("background-color: #0077CC;"
                                  "color: white;"
                                  "border: 1px solid #0055AA;"
                                  "border-radius: 5px;"
                                  "padding: 5px 10px;");
    connect(sign_up_button, &QPushButton::clicked, this, &client_main_window::on_sign_up);

    QVBoxLayout *sign_up_layout = new QVBoxLayout();
    sign_up_layout->addLayout(first_name_layout);
    sign_up_layout->addLayout(last_name_layout);
    sign_up_layout->addLayout(phone_number_layout);
    sign_up_layout->addLayout(password_layout);
    sign_up_layout->addLayout(password_confirm_layout);
    sign_up_layout->addLayout(secret_question_layout);
    sign_up_layout->addLayout(secret_answer_layout);
    sign_up_layout->addWidget(sign_up_button);

    QGroupBox *group_box_2 = new QGroupBox();
    group_box_2->setLayout(sign_up_layout);

    QGridLayout *sign_up_grid = new QGridLayout(sign_up_widget);
    sign_up_grid->addWidget(group_box_2, 0, 0, 1, 1, Qt::AlignCenter);

    /*-----------------------------------¬------------------------------------------------------------------------------------------------------------------------------------*/

    QWidget *chat_widget = new QWidget();

    QPushButton *server = new QPushButton("Talk to an Agent/Server", this);
    connect(server, &QPushButton::clicked, this, [=]()
            { QWidget *wid = _window_map.value("Server", this);
            if (wid)
             _stack->setCurrentIndex(_stack->indexOf(wid)); });

    QLabel *name = new QLabel("My Name: ", chat_widget);
    _name = new QLineEdit(chat_widget);
    _name->setPlaceholderText("INSERT YOUR NAME THEN PRESS ENTER");
    connect(_name, &QLineEdit::returnPressed, this, &client_main_window::on_name_changed);

    QHBoxLayout *hbox_2 = new QHBoxLayout();
    hbox_2->addWidget(name);
    hbox_2->addWidget(_name);

    QPushButton *create_group = new QPushButton("New Group +", this);
    connect(create_group, &QPushButton::clicked, this, &client_main_window::create_group);

    _list = new QListWidget(chat_widget);
    _list->setSelectionMode(QAbstractItemView::NoSelection);
    _list->setMinimumWidth(200);
    _list->setFont(QFont("Arial", 20));
    _list->setItemDelegate(new separator_delegate(_list));
    connect(_list, &QListWidget::itemClicked, this, &client_main_window::on_item_clicked);

    QLabel *chats_label = new QLabel("CHATS", chat_widget);

    QLabel *fr_list = new QLabel("Start New conversation", this);
    _friend_list = new QComboBox(this);
    connect(_friend_list, &QComboBox::textActivated, this, &client_main_window::new_conversation);

    QHBoxLayout *hbox_3 = new QHBoxLayout();
    hbox_3->addWidget(fr_list);
    hbox_3->addWidget(_friend_list);
    hbox_3->addWidget(create_group);

    _search_phone_number = new QLineEdit(this);
    _search_phone_number->setPlaceholderText("ADD PEOPLE VIA PHONE NUMBER, THEN PRESS ENTER");
    connect(_search_phone_number, &QLineEdit::returnPressed, this, [=]()
            { _server_wid->add_friend(_search_phone_number->text()); });

    QVBoxLayout *VBOX_2 = new QVBoxLayout(chat_widget);
    VBOX_2->addLayout(hbox_2);
    VBOX_2->addLayout(hbox_3);
    VBOX_2->addWidget(server);
    VBOX_2->addWidget(chats_label);
    VBOX_2->addWidget(_list);
    VBOX_2->addWidget(_search_phone_number);

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

void client_main_window::on_sign_up()
{
    if (_insert_phone_number->text().toInt() == 0)
    {
        _insert_phone_number->setStyleSheet("border: 1px solid red;");
        return;
    }

    _insert_phone_number->setStyleSheet("border: 1px solid gray;");
    if (_insert_password->text().isEmpty())
    {
        _insert_password->setStyleSheet("border: 1px solid red;");
        return;
    }

    _insert_password->setStyleSheet("border: 1px solid gray;");

    if (_insert_password->text().compare(_insert_password_confirmation->text()))
    {
        _insert_password_confirmation->setStyleSheet("border: 1px solid red;");
        return;
    }

    _insert_password_confirmation->setStyleSheet("border: 1px solid gray;");

    if (_insert_secret_question->text().isEmpty())
    {
        _insert_secret_question->setStyleSheet("border: 1px solid red;");
        return;
    }

    _insert_secret_question->setStyleSheet("border: 1px solid gray;");

    if (_insert_secret_answer->text().isEmpty())
    {
        _insert_secret_question->setStyleSheet("border: 1px solid red;");
        return;
    }

    _insert_secret_question->setStyleSheet("border: 1px solid gray;");

    QString info = QString("First Name : %1\nLast Name : %2\nPhone Number : %3\nSecret Question : %4\nSecret Answer : %5")
                       .arg(_insert_first_name->text())
                       .arg(_insert_last_name->text())
                       .arg(_insert_phone_number->text())
                       .arg(_insert_secret_question->text())
                       .arg(_insert_secret_answer->text());

    QInputDialog *input_dialog = new QInputDialog(this);
    input_dialog->setWindowTitle("Information Review");
    input_dialog->setLabelText("Please Review the Information below carefully:");
    input_dialog->setOptions(QInputDialog::UsePlainTextEditForTextInput);
    input_dialog->setTextValue(info);

    connect(input_dialog, &QInputDialog::finished, this, [=](int result)
            {
                if (result == QDialog::Accepted)
                {
                    if (!_server_wid)
                    {
                        _server_wid = new client_chat_window(_user_phone_number->text(), this);
                        QTimer::singleShot(2000, this, [=]()
                                           { _server_wid->_client->send_sign_up(_insert_phone_number->text(), _insert_first_name->text(), _insert_last_name->text(), _insert_password->text(), _insert_secret_question->text(), _insert_secret_answer->text()); });
                    }
                    _status_bar->showMessage(QString("Account Created Successfully"), 10000);
                    _stack->setCurrentIndex(1);
                }

                input_dialog->deleteLater(); });

    input_dialog->open();
}

void client_main_window::on_login_request(const QString &hashed_password, bool true_or_false, const QHash<int, QHash<QString, int>> &list_g, const QList<QString> &online_friends, const QHash<int, QVector<QString>> &messages, const QHash<int, QHash<QString, QByteArray>> &binary_datas)
{
    if (hashed_password.isEmpty())
    {
        _user_phone_number->setStyleSheet("border: 1px solid red");

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

        connect(_server_wid, &client_chat_window::is_typing_received, this, [=](const QString &sender)
                { _status_bar->showMessage(QString("%1 is typing...").arg(sender), 3000); });

        connect(_server_wid, &client_chat_window::socket_disconnected, this, [=]()
                { _stack->setDisabled(true); _status_bar->showMessage("SERVER DISCONNECTED YOU", 999999); });

        connect(_server_wid, &client_chat_window::data_received_sent, this, [=](const QString &client_name)
                { add_on_top(client_name); });

        connect(_server_wid, &client_chat_window::saving_file, this, [=](const QString &message)
                { 
                    if (message.compare("file saved"))
                    _status_bar->showMessage(QString("%1").arg(message), 3000); 
                    else
                        _status_bar->showMessage(QString("%1").arg(message), 30000); });

        if (list_g.isEmpty())
            return;

        QIcon offline_icon = create_dot_icon(Qt::red, 10);
        QIcon online_icon = create_dot_icon(Qt::green, 10);

        for (const int &conversation_ID : list_g.keys())
        {
            const QHash<QString, int> &list = list_g.value(conversation_ID);

            QVector<QString> message = messages.value(conversation_ID);

            QHash<QString, QByteArray> binary_data = binary_datas.value(conversation_ID);

            for (const QString &name : list.keys())
            {
                _friend_list->addItem(name);

                QIcon valid_icon = online_friends.contains(name) ? online_icon : offline_icon;

                _friend_list->setItemIcon(_friend_list->count() - 1, valid_icon);

                if (_window_map.contains(name))
                    continue;

                client_chat_window *wid = new client_chat_window(conversation_ID, QString::number(list.value(name)), name, this);
                wid->retrieve_conversation(message, binary_data);

                connect(wid, &client_chat_window::swipe_right, this, &client_main_window::on_swipe_right);
                connect(wid, &client_chat_window::data_received_sent, this, [=](QString first_name)
                        { add_on_top(first_name); });

                wid->window_name(name);

                _window_map.insert(name, wid);

                _stack->addWidget(wid);

                if (!messages.isEmpty())
                {
                    QListWidgetItem *item = new QListWidgetItem(name);
                    item->setIcon(valid_icon);

                    _list->addItem(item);
                }
            }
        }
        _user_phone_number->clear();
        _user_password->clear();
    }
    else
    {
        _user_password->setStyleSheet("border: 1px solid red");

        _status_bar->showMessage(QString("The entered password is incorrect, Verify it and try again"), 5000);

        return;
    }

    _user_password->setStyleSheet("border: 1px solid gray");
}

void client_main_window::on_name_changed()
{
    if (!_name->text().isEmpty())
    {
        for (QWidget *win : _window_map)
        {
            client_chat_window *wid = qobject_cast<client_chat_window *>(win);
            wid->set_name(_name->text());
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
    QWidget *win = _window_map.value(client_name);
    if (win)
    {
        QIcon offline_icon = create_dot_icon(Qt::red, 10);

        int index = _friend_list->findText(client_name);
        if (index != -1)
            _friend_list->setItemIcon(index, offline_icon);

        QList<QListWidgetItem *> items = _list->findItems(client_name, Qt::MatchExactly);
        if (!items.isEmpty())
            items.first()->setIcon(offline_icon);

        _status_bar->showMessage(QString("%1 is Disconnected").arg(client_name), 5000);
    }
}

void client_main_window::on_client_connected(const QString &client_name)
{
    QWidget *win = _window_map.value(client_name);
    if (win)
    {
        QIcon online_icon = create_dot_icon(Qt::green, 10);

        int index = _friend_list->findText(client_name);
        if (index != -1)
            _friend_list->setItemIcon(index, online_icon);

        QList<QListWidgetItem *> items = _list->findItems(client_name, Qt::MatchExactly);
        if (!items.isEmpty())
            items.first()->setIcon(online_icon);

        _status_bar->showMessage(QString("%1 is Connected").arg(client_name), 5000);
    }
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
            wid->message_received(text, time);

            add_on_top(sender);
        }
        else
            qDebug() << "client_main_window ---> on_text_message_received() --> ERROR CASTING THE WIDGET:";
    }
}

void client_main_window::on_item_clicked(QListWidgetItem *item)
{
    QWidget *wid = _window_map.value(item->text(), this);
    if (wid)
        _stack->setCurrentIndex(_stack->indexOf(wid));
    else
        qDebug() << "client_main_window--> on_item_clicked()--> Widget not found in _window_map for name:" << item->text();
}

void client_main_window::on_client_name_changed(const QString &old_name, const QString &client_name)
{
    QWidget *win = _window_map.value(old_name);
    if (win)
    {
        _window_map.remove(old_name);
        _window_map.insert(client_name, win);

        QList<QListWidgetItem *> items = _list->findItems(old_name, Qt::MatchExactly);
        if (!items.empty())
            items.first()->setText(client_name);
        else
            qDebug() << "client_main_window ---> on_client_name_changed() ---> Client Name not Found in the _list: " << old_name;

        int index = _friend_list->findText(old_name);
        if (index != -1)
        {
            _friend_list->removeItem(index);

            _friend_list->insertItem(index, client_name);
        }

        client_chat_window *wind = qobject_cast<client_chat_window *>(win);
        if (wind)
            wind->window_name(client_name);
        else
            qDebug() << "client_main_window ---> on_client_name_changed() ---> ERROR CASTING THE WIDGET:";
    }
}

void client_main_window::on_swipe_right()
{
    if (_stack->currentIndex() > 2)
        _stack->setCurrentIndex(2);
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

        client_chat_window *wid = new client_chat_window(conversation_ID, _search_phone_number->text(), name, this);
        connect(wid, &client_chat_window::swipe_right, this, &client_main_window::on_swipe_right);
        connect(wid, &client_chat_window::data_received_sent, this, [=](const QString &first_name)
                { add_on_top(first_name); });

        wid->window_name(name);

        _stack->addWidget(wid);

        _window_map.insert(name, wid);

        _status_bar->showMessage(QString("%1 known as %2 is now in your friend_list").arg(_search_phone_number->text(), name), 5000);
    }
    else
        _status_bar->showMessage(QString("%1 known as %2 is already in your friend_list").arg(_search_phone_number->text(), name), 5000);

    _search_phone_number->clear();
}

void client_main_window::new_conversation(const QString &name)
{
    QWidget *wid = _window_map.value(name, this);
    if (wid)

        _stack->setCurrentIndex(_stack->indexOf(wid));
    else
        qDebug() << "client_main_window--> new_conversation()--> Widget not found in _window_map for name:" << name;
}

void client_main_window::on_client_added_you(const int &conversation_ID, const QString &name, const QString &ID)
{
    if (_friend_list->findText(name, Qt::MatchExactly) == -1)
    {
        _friend_list->addItem(name);

        _friend_list->setItemIcon(_friend_list->count() - 1, create_dot_icon(Qt::green, 10));

        client_chat_window *wid = new client_chat_window(conversation_ID, ID, name, this);
        if (!wid)
        {
            qDebug() << "couldn't create a chat for the seach number";
            return;
        }

        connect(wid, &client_chat_window::swipe_right, this, &client_main_window::on_swipe_right);
        connect(wid, &client_chat_window::data_received_sent, this, [=](const QString &first_name)
                { add_on_top(first_name); });

        wid->window_name(name);

        _stack->addWidget(wid);

        _window_map.insert(name, wid);

        _status_bar->showMessage(QString("%1 added You").arg(name), 5000);
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
            add_on_top(sender);
        }
        else
            qDebug() << "client_main_window ---> on_text_message_received() --> ERROR CASTING THE WIDGET:";
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
            add_on_top(sender);
        }
        else
            qDebug() << "client_main_window ---> on_text_message_received() --> ERROR CASTING THE WIDGET:";
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
        else
            qDebug() << "client_main_window ---> on_delete_message() --> ERROR CASTING THE WIDGET:";
    }
}

void client_main_window::create_group()
{
    QInputDialog *input_dialog = new QInputDialog(this);
    input_dialog->setWindowTitle("Group Name");
    input_dialog->setLabelText("Enter Group Name");

    connect(input_dialog, &QInputDialog::finished, this, [=](int result)
            {
                if (result == QDialog::Accepted)
                {
                    QString group_name = input_dialog->textValue();
                    if (!group_name.isEmpty())
                    {
                        QStringList friends_name;
                        for (int i = 0; i < _friend_list->count(); i++)
                            friends_name << _friend_list->itemText(i);

                        select_group_member *group_members = new select_group_member(friends_name, this);
                        group_members->setFixedSize(300, 400);

                        connect(group_members, &QDialog::accepted, this, [=]()
                                {
                                    QStringList names = group_members->name_selected();
                                    _server_wid->_client->send_create_new_group(_server_wid->my_name(), names, group_name);
                                    connect(_server_wid, &client_chat_window::new_group_ID, this, [=](const int &conversation_ID)
                                            {   client_chat_window *wid = new client_chat_window(conversation_ID, group_name, group_name, this);
                                                connect(wid, &client_chat_window::swipe_right, this, &client_main_window::on_swipe_right);
                                                wid->window_name(group_name);

                                                _window_map.insert(group_name, wid);

                                                _stack->addWidget(wid); 

                                                _friend_list->addItem(group_name); });
                                    });


                        group_members->open();
                    }
                } });

    input_dialog->open();
}

/*-------------------------------------------------------------------- Functions --------------------------------------------------------------*/

void client_main_window::add_on_top(const QString &client_name)
{
    QList<QListWidgetItem *> items = _list->findItems(client_name, Qt::MatchExactly);
    if (!items.empty())
    {
        QListWidgetItem *item_to_replace = items.first();

        _list->takeItem(_list->row(item_to_replace));
        _list->insertItem(0, item_to_replace);
    }
    else
        _list->insertItem(0, client_name);
}

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