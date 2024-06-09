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
    client_chat_window(const int &group_ID, const QString &group_name, const QStringList &group_members, QWidget *parent = nullptr);

    void set_name(const QString &insert_name);

    void window_name(const QString &name);
    void message_received(const QString &message, const QString &time, const QString &sender = QString());

    void retrieve_conversation(const QStringList &messages, const QHash<QString, QByteArray> &binary_data);
    void retrieve_group_conversation(const QStringList &group_messages, const QHash<QString, QByteArray> &group_binary_data);

    void add_file(const QString &path, bool is_mine, const QString &time, const QString &sender = QString());
    void add_audio(const QString &audio_name, bool is_mine, const QString &time, const QString &sender = QString());
    void add_friend(const QString &ID);

    void delete_message_received(const QString &time);

    static client_manager *_client;

    void message_deleted(const QString &time);

    Swipeable_list_widget *_list;

    QStringList _group_members = QStringList();
    QString _group_name = QString();

    DisplayWidget *_display_widget;

    QString my_name();

    void create_new_group(QStringList group_members, QString group_name);

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

    bool is_recording = false;

    bool is_playing = false;

    int paused_position = 0;

    int _group_ID;

    static int color_counter;
    static QString last_sender;
    static QColor last_color;
    static QColor colors[];

    void ask_microphone_permission();

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void set_up_window();
    void set_up_window_2();

    void set_retrieve_message_window(const QString &type, const QString &content, const QByteArray &file_data, const QString &date_time, bool true_or_false, const QString &sender = QString());

signals:
    void swipe_right();

    void client_name_changed(const QString &old_name, const QString &client_name);

    void client_connected(const QString &client_name);
    void client_disconnected(const QString &client_name);

    void text_message_received(const QString &sender, const QString &message, const QString &time);
    void is_typing_received(const QString &sender);

    void socket_disconnected();

    void update_button_file();

    void data_received_sent(const QString &client_name);

    void client_added_you(const int &conversation_ID, const QString &name, const QString &ID);
    void lookup_friend_result(const int &conversation_ID, const QString &name, bool true_or_false);

    void audio_received(const QString &sender, const QString &audio_name, const QString &time);
    void file_received(const QString &sender, const QString &file_name, const QString &time);

    void login_request(const QString &hashed_password, bool true_or_false, const QHash<int, QHash<QString, int>> &friend_list, const QStringList &online_friends, const QHash<int, QStringList> &messages, const QHash<int, QHash<QString, QByteArray>> &binary_data, const QHash<int, QString> &group_list, const QHash<int, QStringList> &group_messages, const QHash<int, QHash<QString, QByteArray>> &group_binary_data, const QHash<int, QStringList> &groups_members);

    void delete_message(const QString &sender, const QString &time);

    void saving_file(const QString &message);

    void new_group_ID(const int &group_ID);

    void item_clicked(const QString &name);

    void added_to_group(const int &group_ID, const QString &adm, const QStringList &group_members, const QString &group_name);

    void group_is_typing_received(const int &group_ID, const QString &group_name, const QString &sender);
    void group_text_received(const int &group_ID, const QString &group_name, const QString &sender, const QString &message, const QString &time);

    void group_audio_received(const int &group_ID, const QString &group_name, const QString &sender, const QString &audio_name, const QString &time);
    void group_file_received(const int &group_ID, const QString &group_name, const QString &sender, const QString &file_name, const QString &time);

private slots:
    void send_message();

    void send_file();

    void start_recording();
    void on_duration_changed(const qint64 &duration);
    void play_audio(const QUrl &source, QPushButton *audio, QSlider *slider);
};

class separator_delegate : public QStyledItemDelegate
{
private:
    QListWidget *m_parent;

public:
    separator_delegate(QListWidget *parent) : QStyledItemDelegate(parent), m_parent(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QStyledItemDelegate::paint(painter, option, index);

        if (index.row() != m_parent->count() - 1)
        {
            painter->save();
            painter->setPen(Qt::green);
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
            painter->restore();
        }
    }
};

class Swipeable_list_widget : public QListWidget
{

private:
    QPoint drag_start_position;
    client_chat_window *_window;

public:
    Swipeable_list_widget(client_chat_window *window, QWidget *parent = nullptr) : QListWidget(parent), _window(window) {}

protected:
    void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton)
            drag_start_position = event->pos();

        QListWidget::mousePressEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            int distance = event->pos().x() - drag_start_position.x();
            if (distance < -50)
            {
                QListWidgetItem *item = itemAt(drag_start_position);
                if (item)
                {
                    QString message = QString("Do you really want to delete this Message: %1?\n Press OK to confirm").arg(item->text());

                    QInputDialog *input_dialog = new QInputDialog(this);
                    input_dialog->setWindowTitle("Deleting Message");
                    input_dialog->setLabelText("Please Review the Information below carefully:");
                    input_dialog->setOptions(QInputDialog::UsePlainTextEditForTextInput);
                    input_dialog->setTextValue(message);

                    connect(input_dialog, &QInputDialog::finished, this, [=](int result)
                            {   
                                if(result == QDialog::Accepted)
                                { 
                                    _window->message_deleted(item->data(Qt::UserRole).toString());
                                    delete _window->_list->takeItem(_window->_list->row(item));
                                }

                                input_dialog->deleteLater(); });

                    input_dialog->open();
                }
            }
        }
        QListWidget::mouseReleaseEvent(event);
    }
};

class ListDialog : public QDialog
{
    Q_OBJECT

private:
    QListWidget *name_list;
    QDialogButtonBox *button_box;

public:
    explicit ListDialog(const QStringList &names, const QString &title, QWidget *parent = nullptr)
        : QDialog(parent)
    {
        setWindowTitle(title);
        resize(200, 200);

        QVBoxLayout *layout = new QVBoxLayout(this);

        name_list = new QListWidget();
        name_list->addItems(names);
        name_list->setSelectionMode(QAbstractItemView::MultiSelection);

        button_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(button_box, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(button_box, &QDialogButtonBox::rejected, this, &QDialog::reject);

        layout->addWidget(name_list);
        layout->addWidget(button_box);

        setLayout(layout);
    }

    QStringList name_selected() const
    {
        QStringList selected;

        for (QListWidgetItem *item : name_list->selectedItems())
            selected << item->text();

        return selected;
    }
};

class DisplayWidget : public QWidget
{
    Q_OBJECT

private:
    QLabel *label;

public:
    DisplayWidget(QWidget *parent = nullptr) : QWidget(parent)
    {
        QHBoxLayout *layout = new QHBoxLayout(this);
        label = new QLabel(this);
        layout->addWidget(label);
        setLayout(layout);
    }

    void setText(const QString &text)
    {
        label->setText(text);
    }
};

class CustomLineEdit : public QLineEdit
{
    Q_OBJECT
signals:
    void focusGained();
    void focusLost();

public:
    explicit CustomLineEdit(QWidget *parent = nullptr) : QLineEdit(parent) {}

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
};