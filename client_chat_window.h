#pragma once

#include "client_manager.h"
#include "chat_line.h"

class separator_delegate;

class Swipeable_list_widget;

class DisplayWidget;

class CustomLineEdit;

class client_chat_window : public QMainWindow
{
    Q_OBJECT
public:
    client_chat_window(QWidget *parent = nullptr);
    client_chat_window(const int &conversation_ID, const QString &destinator, const QString &name, QWidget *parent = nullptr);
    client_chat_window(const int &group_ID, const QString &group_name, const QStringList &group_members, const QString &adm, QWidget *parent = nullptr);

    void set_name(const QString &insert_name);

    void window_name(const QString &name);
    void text_message_background(const QString &message, const QString &time, const QString &sender = QString(), bool true_or_false = false);

    void retrieve_conversation(const QStringList &messages);
    void retrieve_group_conversation(const QStringList &group_messages);

    void add_file(const QString &path, bool is_mine, const QString &time, const QString &sender = QString());
    void add_audio(const QString &audio_name, bool is_mine, const QString &time, const QString &sender = QString());

    void delete_message_received(const QString &time);
    void message_deleted(const QString &time);

    void disable_chat();
    void enable_chat();

    void delete_account();

    int time_difference(const QString &date_time1, const QString &date_time2);

    static void set_window_blur(QWidget *window, bool blur);

    void in_chat();

    static client_manager *_client;

    Swipeable_list_widget *_list;

    QStringList _group_members = QStringList();
    QString _group_name = QString();

    int _unread_messages = 0;
    QString _last_message = QString();

    QString my_name();

private:
    QStatusBar *_status_bar;

    QString _destinator_name;
    QString _destinator = "Server";
    QString _window_name = "Server";

    int _conversation_ID;

    QString _my_ID;

    static QString _my_name;
    static QString _insert_name;

    CustomLineEdit *_insert_message;

    QPushButton *_send_file_button;
    QPushButton *_send_button;
    QPushButton *_record_button;
    QHBoxLayout *_buttons;

    QLabel *_duration_label;

    QPoint _drag_start_position;
    bool _dragging = false;

    std::vector<int> _conversation_list;

    QHBoxLayout *_hbox;
    QDir _dir;

    QMediaCaptureSession *_session;
    QAudioInput *_audio_input;
    QMediaRecorder *_recorder;

    QMediaPlayer *_player;
    QAudioOutput *_audio_output;

    static bool _is_recording;
    static bool _is_playing;
    static int _paused_position;

    int _group_ID;

    static int _color_counter;
    static QString _last_sender;
    static QColor _last_color;
    static QColor _colors[];

    QString _adm;

    void ask_microphone_permission();

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void set_up_window();
    void set_up_window_2();

    void set_retrieve_message_window(const QString &type, const QString &content, const QString &date_time, bool true_or_false, const QString &sender = QString());

    void audio_file_message_background(QWidget *wid, const bool &is_mine, const QString &sender, const QString &time, QVBoxLayout *vbox, QSlider *slider = nullptr);

signals:
    void swipe_right();

    void client_name_changed(const QString &old_name, const QString &client_name);

    void client_connected(const QString &client_name);
    void client_disconnected(const QString &client_name);

    void text_message_received(const QString &sender, const QString &message, const QString &time);
    void is_typing_received(const QString &sender);

    void socket_disconnected();

    void update_button_file();

    void data_sent(const QString &client_name, const QString &last_message, const int &unread_messages);

    void client_added_you(const int &conversation_ID, const QString &name, const QString &ID);
    void lookup_friend_result(const int &conversation_ID, const QString &name, bool true_or_false);

    void audio_received(const QString &sender, const QString &audio_name, const QString &time);
    void file_received(const QString &sender, const QString &file_name, const QString &time);

    void login_request(const QString &hashed_password, bool true_or_false, const QHash<int, QHash<QString, QString>> &friend_list, const QStringList &online_friends, const QHash<int, QStringList> &messages, const QHash<int, QHash<int, QString>> &group_list, const QHash<int, QStringList> &group_messages, const QHash<int, QStringList> &groups_members);

    void delete_message(const QString &sender, const QString &time);

    void new_group_ID(const int &group_ID);

    void item_clicked(const QString &name);

    void added_to_group(const int &group_ID, const QString &adm, const QStringList &group_members, const QString &group_name);

    void group_is_typing_received(const int &group_ID, const QString &group_name, const QString &sender);
    void group_text_received(const int &group_ID, const QString &group_name, const QString &sender, const QString &message, const QString &time);

    void group_audio_received(const int &group_ID, const QString &group_name, const QString &sender, const QString &audio_name, const QString &time);
    void group_file_received(const int &group_ID, const QString &group_name, const QString &sender, const QString &file_name, const QString &time);

    void removed_from_group(const int &group_ID, const QString &group_name, const QString &adm);

private slots:
    void send_message();

    void send_file();

    void on_settings();

    void start_recording();
    void on_duration_changed(const qint64 &duration);
    void play_audio(const QUrl &source, QPushButton *audio, QSlider *slider);
};

class SeparatorDelegate : public QStyledItemDelegate
{
private:
    QListWidget *m_parent;

public:
    explicit SeparatorDelegate(QListWidget *parent) : QStyledItemDelegate(parent), m_parent(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        painter->save();

        QRect rect = option.rect.adjusted(2, 2, -2, -2);

        painter->setBrush(QBrush(Qt::white));
        painter->setPen(Qt::NoPen);
        painter->drawRoundedRect(rect, 50, 50);

        QStyleOptionViewItem adjustedOption = option;
        adjustedOption.rect = rect;
        adjustedOption.displayAlignment = Qt::AlignCenter;
        QStyledItemDelegate::paint(painter, adjustedOption, index);

        painter->restore();
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QSize size = QStyledItemDelegate::sizeHint(option, index);
        size.setHeight(size.height() + 2);

        return size;
    }
};

class ListDialog : public QDialog
{
    Q_OBJECT

private:
    QListWidget *name_list;
    QDialogButtonBox *button_box;
    QLineEdit *_input_line;

protected:
    void showEvent(QShowEvent *event) override
    {
        if (parentWidget())
            client_chat_window::set_window_blur(parentWidget(), true);

        QDialog::showEvent(event);
    }

    void hideEvent(QHideEvent *event) override
    {
        if (parentWidget())
            client_chat_window::set_window_blur(parentWidget(), false);

        QDialog::hideEvent(event);
    }

public:
    explicit ListDialog(const QStringList &names, const QString &title, QWidget *parent = nullptr, bool true_or_false = false)
        : QDialog(parent)
    {
        setWindowTitle(title);
        resize(200, 300);
        setStyleSheet("background-color: white;"
                      " border: 1px solid #4A90E2;"
                      " padding: 5px 10px;"
                      " border-radius: 10px;");

        QVBoxLayout *layout = new QVBoxLayout(this);

        name_list = new QListWidget();
        name_list->addItems(names);
        name_list->setSelectionMode(QAbstractItemView::MultiSelection);

        layout->addWidget(name_list);

        if (true_or_false)
        {
            QFormLayout *form_layout = new QFormLayout;
            _input_line = new QLineEdit(this);

            form_layout->addRow("Enter Value:", _input_line);
            layout->addLayout(form_layout);
        }
        else
            _input_line = nullptr;

        button_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(button_box, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(button_box, &QDialogButtonBox::rejected, this, &QDialog::reject);

        layout->addWidget(button_box);

        setLayout(layout);

        QString styleSheet = R"(
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
            QDialogButtonBox {
                background-color: #F5F5F5;
            }
            QPushButton {
                background-color: #4A90E2;
                color: white;
                border: 1px solid #4A90E2;
                padding: 5px 10px;
                border-radius: 10px;
            }
            QPushButton:pressed {
                background-color: #2C5AA0;
                border: 1px solid #2C5AA0;
            }
        )";

        setStyleSheet(styleSheet);
    }

    QStringList name_selected() const
    {
        QStringList selected;

        for (QListWidgetItem *item : name_list->selectedItems())
            selected << item->text();

        return selected;
    }

    QString value_entered() const { return (_input_line) ? _input_line->text() : QString(); }
};

class Swipeable_list_widget : public QListWidget
{
private:
    QPoint drag_start_position;
    client_chat_window *_window;

public:
    explicit Swipeable_list_widget(client_chat_window *window, QWidget *parent = nullptr) : QListWidget(parent), _window(window) {}

    void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton)
            drag_start_position = event->pos();

        QListWidget::mousePressEvent(event);
    }

    static QString item_message(QListWidgetItem *item)
    {
        QWidget *widget = item->listWidget()->itemWidget(item);
        if (widget)
        {
            QVBoxLayout *layout = widget->findChild<QVBoxLayout *>();
            if (layout)
            {
                QLabel *message = qobject_cast<QLabel *>(layout->itemAt(0)->widget());
                if (message)
                    return message->text();
            }
        }

        return QString();
    }

    void mouseReleaseEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            int distance = event->pos().x() - drag_start_position.x();
            if (distance < -50)
            {
                QListWidgetItem *item = QListWidget::itemAt(drag_start_position);

                if (item->background() != QBrush(QColorConstants::Svg::lightskyblue))
                    return;

                QStringList info;
                info << "Do you really want to delete this Message: "
                     << item_message(item)
                     << " Press OK to confirm";

                ListDialog *dialog = new ListDialog(info, "Delete Message", this);

                connect(dialog, &QDialog::accepted, this, [=]()
                        {   _window->message_deleted(item->data(Qt::UserRole).toString());
                            dialog->deleteLater(); });

                dialog->open();

                QListWidget::mouseReleaseEvent(event);
            }
        }
    }
};

class DisplayWidget : public QWidget
{
    Q_OBJECT
private:
    QLineEdit *_lineEdit;

public:
    explicit DisplayWidget(QWidget *parent = nullptr) : QWidget(parent)
    {
        QHBoxLayout *layout = new QHBoxLayout(this);

        _lineEdit = new QLineEdit(this);
        layout->addWidget(_lineEdit);

        setLayout(layout);
    }

public slots:
    void setText(const QString &text)
    {
        _lineEdit->setText(text);
        _lineEdit->setCursorPosition(_lineEdit->text().length());
    }
};

class CustomLineEdit : public QLineEdit
{
    Q_OBJECT
protected:
    void focusInEvent(QFocusEvent *event) override
    {
        QLineEdit::focusInEvent(event);
        emit focusGained();
    }

    void focusOutEvent(QFocusEvent *event) override
    {
        QLineEdit::focusOutEvent(event);
        emit focusLost();
    }

public:
    explicit CustomLineEdit(QWidget *parent = nullptr) : QLineEdit(parent) {}

signals:
    void focusGained();
    void focusLost();
};