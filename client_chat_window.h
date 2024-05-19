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

class Swipeable_list_widget : public QListWidget
{

private:
    QPoint drag_start_position;

public:
    Swipeable_list_widget(QWidget *parent = nullptr) : QListWidget(parent) {}

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
                    delete item;
            }
        }
        QListWidget::mouseReleaseEvent(event);
    }
};

class client_chat_window : public QMainWindow
{
    Q_OBJECT
public:
    client_chat_window(QString my_ID, QWidget *parent = nullptr);
    client_chat_window(int conversation_ID, QString destinator, QString name, QWidget *parent = nullptr);

    void set_name(QString insert_name);

    void window_name(QString name);
    void message_received(QString message);
    void retrieve_conversation(QVector<QString> &messages, QHash<QString, QByteArray> &binary_data);

    void add_file(QString path, bool is_mine = false, QString date_time = "");
    void add_audio(const QUrl &source, bool is_mine = false, QString date_time = "");
    void add_friend(QString ID);

    static client_manager *_client;

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

signals:
    void swipe_right();

    void client_name_changed(QString old_name, QString client_name);

    void client_connected(QString client_name);
    void client_disconnected(QString client_name);

    void text_message_received(QString sender, QString message);
    void is_typing_received(QString sender);

    void socket_disconnected();

    void update_button_file();

    void data_received_sent(QString client_name);

    void client_added_you(int conversation_ID, QString name, QString ID);
    void lookup_friend_result(int conversation_ID, QString name);

    void audio_received(QString sender, QString path);
    void file_saved(QString sender, QString path);

    void item_deleted(QListWidgetItem *item);

    void login_request(QString hashed_password, bool true_or_false, QHash<int, QHash<QString, int>> friend_list, QList<QString> online_friends, QHash<int, QVector<QString>> messages, QHash<int, QHash<QString, QByteArray>> binary_data);

private slots:
    void send_message();

    void send_file();
    void send_file_client();

    void folder();
    void on_file_saved(QString path);

    void on_init_receiving_file(QString file_name, qint64 file_size);
    void on_init_receiving_file_client(QString sender, QString ID, QString file_name, qint64 file_size);

    void start_recording();
    void on_duration_changed(qint64 duration);
    void play_audio(const QUrl &source, QPushButton *audio, QSlider *slider);

    void on_item_deleted(QListWidgetItem *item);
};