#pragma once

#include "client_chat_window.h"
#include <QtWidgets>
#include <QtCore>
#include <QtMultimedia>
#include <QWebSocket>
class client_main_window : public QMainWindow
{
    Q_OBJECT
public:
    client_main_window(QWidget *parent = nullptr);
    ~client_main_window();

private:
    QStackedWidget *_stack;

    QStatusBar *_status_bar;

    QTabWidget *_tabs;

    QPoint drag_start_position;
    bool dragging = false;

    static client_chat_window *_server_wid;
    static QHash<QString, QWidget *> _window_map;

    QLineEdit *_name;
    QLineEdit *_user_phone_number;
    QLineEdit *_user_password;
    QLineEdit *_search_phone_number;

    QLineEdit *_insert_first_name;
    QLineEdit *_insert_last_name;
    QLineEdit *_insert_phone_number;
    QLineEdit *_insert_password;
    QLineEdit *_insert_password_confirmation;
    QLineEdit *_insert_secret_question;
    QLineEdit *_insert_secret_answer;

    Swipeable_list_widget *_list;

    QComboBox *_friend_list;

    void add_on_top(const QString &client_name);
    QIcon create_dot_icon(const QColor &color, int size);

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

signals:
    void swipe_right();

private slots:
    void on_swipe_right();

    void on_sign_in();
    void on_login_request(QString hashed_password, bool true_or_false, QHash<int, QHash<QString, int>> friend_list, QList<QString> online_friends, QHash<int, QVector<QString>> messages, QHash<int, QHash<QString, QByteArray>> binary_data);

    void on_client_name_changed(QString old_name, QString client_name);
    void on_client_disconnected(QString client_name);

    void on_client_connected(QString client_name);

    void on_text_message_received(QString sender, QString message);
    void on_name_changed();

    void on_item_clicked(QListWidgetItem *item);
    void new_conversation(const QString &name);

    void on_client_added_you(int conversation_ID, QString name, QString ID);
    void on_lookup_friend_result(int conversation_ID, QString full_name);

    void on_audio_received(QString sender, QString path);
    void on_file_saved(QString sender, QString path);

    void on_item_deleted(QListWidgetItem *item);
};
