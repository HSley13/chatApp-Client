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

    QStringList _group_members;
    QString _group_name;

    QListWidget *_list;

    QComboBox *_friend_list;

    void add_on_top(const QString &client_name);
    QIcon create_dot_icon(const QColor &color, int size);

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

signals:
    void swipe_right();

private slots:
    void on_swipe_right();

    void on_sign_up();
    void on_login_request(const QString &hashed_password, bool true_or_false, const QHash<int, QHash<QString, int>> &friend_list, const QList<QString> &online_friends, const QHash<int, QVector<QString>> &messages, const QHash<int, QHash<QString, QByteArray>> &binary_data);

    void on_client_name_changed(const QString &old_name, const QString &client_name);
    void on_client_disconnected(const QString &client_name);

    void on_client_connected(const QString &client_name);

    void on_text_message_received(const QString &sender, const QString &message, const QString &time);
    void on_name_changed();

    void on_item_clicked(QListWidgetItem *item);
    void new_conversation(const QString &name);

    void on_client_added_you(const int &conversation_ID, const QString &name, const QString &ID);
    void on_lookup_friend_result(const int &conversation_ID, const QString &full_name, bool true_or_false);

    void on_audio_received(const QString &sender, const QString &audio_name, const QString &time);
    void on_file_received(const QString &sender, const QString &file_name, const QString &time);

    void on_delete_message(const QString &sender, const QString &time);

    void create_group();

    void on_new_group(const int &conversation_ID);
};

#include <QInputDialog>
#include <QListWidget>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>

class select_group_member : public QDialog
{
    Q_OBJECT

private:
    QListWidget *name_list;
    QDialogButtonBox *button_box;

public:
    explicit select_group_member(const QStringList &names, QWidget *parent = nullptr)
        : QDialog(parent)
    {
        setWindowTitle("Select Group Members");

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
