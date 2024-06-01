#pragma once

#include "client_manager.h"
#include "chat_line.h"
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

class Swipeable_list_widget;
class client_chat_window : public QMainWindow
{
    Q_OBJECT
public:
    client_chat_window(const QString &my_ID, QWidget *parent = nullptr);
    client_chat_window(const int &conversation_ID, const QString &destinator, const QString &name, QWidget *parent = nullptr);

    void set_name(const QString &insert_name);

    void window_name(const QString &name);
    void message_received(const QString &message, const QString &time);
    void retrieve_conversation(QVector<QString> &messages, QHash<QString, QByteArray> &binary_data);

    void add_file(const QString &path, bool is_mine, const QString &time);
    void add_audio(const QString &audio_name, bool is_mine, const QString &time);
    void add_friend(const QString &ID);

    void delete_message_received(const QString &time);

    static client_manager *_client;

    void message_deleted(const QString &time);

    Swipeable_list_widget *_list;

private:
    QStatusBar *_status_bar;

    QString _destinator_name;
    QString _destinator = "Server";
    QString _window_name = "Server";

    int _conversation_ID;

    QString _my_ID;

    static QString _my_name;
    static QString _insert_name;

    QLineEdit *_insert_message;

    QPushButton *_send_file_button;

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

    void ask_microphone_permission();

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    QString my_name();

    void set_up_window();

    void set_retrieve_message_window(const QString &type, const QString &content, const QByteArray &file_data, const QString &date_time, bool true_or_false);

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

    void login_request(const QString &hashed_password, bool true_or_false, const QHash<int, QHash<QString, int>> &friend_list, const QList<QString> &online_friends, const QHash<int, QVector<QString>> &messages, const QHash<int, QHash<QString, QByteArray>> &binary_data);

    void delete_message(const QString &sender, const QString &time);

private slots:
    void send_message();

    void send_file();

    void on_file_saved(const QString &path);

    void on_init_send_file_received(const QString &sender, const QString &sender_ID, const QString &file_name, const qint64 &file_size);

    void start_recording();
    void on_duration_changed(const qint64 &duration);
    void play_audio(const QUrl &source, QPushButton *audio, QSlider *slider);
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