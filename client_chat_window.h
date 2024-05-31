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
    client_chat_window(QString my_ID, QWidget *parent = nullptr);
    client_chat_window(int conversation_ID, QString destinator, QString name, QWidget *parent = nullptr);

    void set_name(QString insert_name);

    void window_name(QString name);
    void message_received(QString message, QString time);
    void retrieve_conversation(QVector<QString> &messages, QHash<QString, QByteArray> &binary_data);

    void add_file(QString path, bool is_mine, QString time);
    void add_audio(QString audio_name, bool is_mine, QString time);
    void add_friend(QString ID);

    void delete_message_received(const QString &time);

    static client_manager *_client;

    void message_deleted(QString time);

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

    Swipeable_list_widget *_list;

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

    void set_retrieve_message_window(QString type, QString content, QByteArray file_data, QString date_time, bool true_or_false);

signals:
    void swipe_right();

    void client_name_changed(QString old_name, QString client_name);

    void client_connected(QString client_name);
    void client_disconnected(QString client_name);

    void text_message_received(QString sender, QString message, QString time);
    void is_typing_received(QString sender);

    void socket_disconnected();

    void update_button_file();

    void data_received_sent(QString client_name);

    void client_added_you(int conversation_ID, QString name, QString ID);
    void lookup_friend_result(int conversation_ID, QString name, bool true_or_false);

    void audio_received(QString sender, QString audio_name, QString time);
    void file_received(QString sender, QString file_name, QString time);

    void item_deleted(QListWidgetItem *item);

    void login_request(QString hashed_password, bool true_or_false, QHash<int, QHash<QString, int>> friend_list, QList<QString> online_friends, QHash<int, QVector<QString>> messages, QHash<int, QHash<QString, QByteArray>> binary_data);

    void delete_message(const QString &sender, const QString &time);

private slots:
    void send_message();

    void send_file();

    void on_init_send_file_received(QString sender, QString sender_ID, QString file_name, qint64 file_size);

    void start_recording();
    void on_duration_changed(qint64 duration);
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
                    QMessageBox *message_box = new QMessageBox(this);
                    message_box->setWindowTitle("Deleting Message");
                    message_box->setText("Please review the information below carefully:");
                    message_box->setInformativeText(QString("Do You really want to delete this Message ? Press OK to confirm"));
                    message_box->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                    message_box->setDefaultButton(QMessageBox::Ok);
                    connect(message_box, &QMessageBox::accepted, this, [=]()
                            { _window->message_deleted(item->data(Qt::UserRole).toString());
                             delete item; });
                }
            }
        }
        QListWidget::mouseReleaseEvent(event);
    }
};