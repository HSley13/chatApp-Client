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
    CustomLineEdit *_search_phone_number;

    QLineEdit *_insert_first_name;
    QLineEdit *_insert_last_name;
    QLineEdit *_insert_phone_number;
    QLineEdit *_insert_password;
    QLineEdit *_insert_password_confirmation;
    QLineEdit *_insert_secret_question;
    QLineEdit *_insert_secret_answer;

    QStringList _group_members;
    QString _group_name;

    QListWidget *_list;

    QComboBox *_group_list;
    QDialog *_group_dialog;
    QComboBox *_friend_list;
    QDialog *_friend_dialog;

    QPushButton *_login_button;

    static QHash<QString, std::function<void()>> _settings_choices;

    void add_on_top(const QString &client_name);

    QIcon create_dot_icon(const QColor &color, int size);
    void set_icon(const QIcon &icon, const QString &client_name);

    void name_changed(const QString &name);

    QStringList authenticate_group_members(const QStringList &group_members_ID);

    void configure_group(const int &group_ID, const QString &group_name, const QStringList &names, const QString &adm);

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void configure_settings_choice();

signals:
    void swipe_right();

private slots:
    void sign_up();
    void login();

    void create_group();

    void on_swipe_right();

    void on_login_request(const QString &hashed_password, bool true_or_false, const QHash<int, QHash<QString, int>> &friend_list, const QStringList &online_friends, const QHash<int, QStringList> &messages, const QHash<int, QHash<QString, QByteArray>> &binary_data, const QHash<int, QHash<int, QString>> &group_list, const QHash<int, QStringList> &group_messages, const QHash<int, QHash<QString, QByteArray>> &group_binary_data, const QHash<int, QStringList> &groups_members);

    void on_settings();

    void on_client_name_changed(const QString &old_name, const QString &client_name);
    void on_client_disconnected(const QString &client_name);

    void on_client_connected(const QString &client_name);

    void on_text_message_received(const QString &sender, const QString &message, const QString &time);

    void on_item_clicked(QListWidgetItem *item);
    void new_conversation(const QString &name);

    void on_client_added_you(const int &conversation_ID, const QString &name, const QString &ID);
    void on_lookup_friend_result(const int &conversation_ID, const QString &full_name, bool true_or_false);

    void on_audio_received(const QString &sender, const QString &audio_name, const QString &time);
    void on_file_received(const QString &sender, const QString &file_name, const QString &time);

    void on_delete_message(const QString &sender, const QString &time);

    void on_added_to_group(const int &group_ID, const QString &adm, const QStringList &group_members, const QString &group_name);

    void on_group_is_typing_received(const int &group_ID, const QString &group_name, const QString &sender);
    void on_group_text_received(const int &group_ID, const QString &group_name, const QString &sender, const QString &message, const QString &time);

    void on_group_audio_received(const int &group_ID, const QString &group_name, const QString &sender, const QString &audio_name, const QString &time);
    void on_group_file_received(const int &group_ID, const QString &group_name, const QString &sender, const QString &file_name, const QString &time);

    void on_removed_from_group(const int &group_ID, const QString &group_name, const QString &adm);
};