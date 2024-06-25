#pragma once

#include "client_chat_window.h"
#include <QtWidgets>
#include <QtCore>
#include <QtMultimedia>
#include <QWebSocket>

class ChatModel;

class ChatDelegate;

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

    QComboBox *_group_list;
    QDialog *_group_dialog;
    QComboBox *_friend_list;
    QDialog *_friend_dialog;

    QPushButton *_login_button;

    static QHash<QString, std::function<void()>> _settings_choices;

    QIcon create_dot_icon(const QColor &color, int size);
    void set_icon(const QIcon &icon, const QString &client_name);

    void name_changed(const QString &name);

    QStringList authenticate_group_members(const QStringList &group_members_ID);

    void configure_group(const int &group_ID, const QString &group_name, const QStringList &names, const QString &adm);

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void configure_settings_choice();

    QListView *_list_view;
    ChatModel *_model;

    QString _active_conversation = QString();

signals:
    void swipe_right();

private slots:
    void sign_up();
    void login();

    void create_group();

    void on_swipe_right();

    void on_login_request(const QString &hashed_password, bool true_or_false, const QHash<int, QHash<QString, QString>> &friend_list, const QStringList &online_friends, const QHash<int, QStringList> &messages, const QHash<int, QHash<int, QString>> &group_list, const QHash<int, QStringList> &group_messages, const QHash<int, QStringList> &groups_members);

    void on_settings();

    void on_client_name_changed(const QString &old_name, const QString &client_name);
    void on_client_disconnected(const QString &client_name);

    void on_client_connected(const QString &client_name);

    void on_text_message_received(const QString &sender, const QString &message, const QString &time);

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

    void on_client_name_clicked(const QString &client_name);
};

class ChatModel : public QStandardItemModel
{
    Q_OBJECT

public:
    enum Roles
    {
        ClientNameRole = Qt::UserRole + 1,
        LastMessageRole,
        UnreadMessagesRole,
        OnlineIconRole
    };

    int _unread_count = 0;

    explicit ChatModel(QObject *parent = nullptr) {}

    void add_chat(const QString &client_name, const QString &last_message, int unread_messages = 0, const QIcon &online_icon = QIcon())
    {
        QStandardItem *item = new QStandardItem();
        item->setData(client_name, ClientNameRole);
        item->setData(last_message, LastMessageRole);
        item->setData(unread_messages, UnreadMessagesRole);
        if (!online_icon.isNull())
            item->setData(QVariant::fromValue(online_icon), OnlineIconRole);

        QStandardItemModel::appendRow(item);
    }

    QModelIndex find_chat_row(const QString &client_name) const
    {
        for (int row = 0; row < QStandardItemModel::rowCount(); row++)
        {
            QModelIndex index = this->index(row, 0);
            if (index.isValid())
            {
                if (index.data(ClientNameRole).toString() == client_name)
                    return index;
            }
        }

        return QModelIndex();
    }

    void update_unread_messages(const QString &client_name, int unread_messages)
    {
        QModelIndex index = find_chat_row(client_name);
        if (index.isValid())
        {
            QStandardItem *item = QStandardItemModel::itemFromIndex(index);
            item->setData(unread_messages, UnreadMessagesRole);
        }
    }

    void update_client_name(const QString &old_client_name, const QString &new_client_name)
    {
        QModelIndex index = find_chat_row(old_client_name);
        if (index.isValid())
        {
            QStandardItem *item = QStandardItemModel::itemFromIndex(index);
            item->setData(new_client_name, ClientNameRole);
        }
    }

    void update_online_icon(const QString &client_name, const QIcon &online_icon)
    {
        QModelIndex index = find_chat_row(client_name);
        if (index.isValid())
        {
            QStandardItem *item = QStandardItemModel::itemFromIndex(index);
            item->setData(QVariant::fromValue(online_icon), OnlineIconRole);
        }
    }

    void add_on_top(const QString &client_name, const QString &last_message, int unread_messages = 0)
    {
        QModelIndex index = find_chat_row(client_name);
        if (index.isValid())
        {
            QStandardItem *takenItem = QStandardItemModel::takeItem(index.row());

            removeRow(index.row());

            takenItem->setData(last_message, LastMessageRole);

            (unread_messages) ? _unread_count += 1 : _unread_count = 0;
            takenItem->setData(_unread_count, UnreadMessagesRole);

            insertRow(0, takenItem);
        }
    }
};

class ChatDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    ChatDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        painter->save();

        QRect rect = option.rect;
        QFont font = QApplication::font();
        font.setPointSize(10);

        QIcon online_icon = index.data(ChatModel::OnlineIconRole).value<QIcon>();
        if (!online_icon.isNull())
        {
            QRect icon_rect = rect.adjusted(10, 5, 10 + 20, 5 + 20);
            online_icon.paint(painter, icon_rect, Qt::AlignLeft);
        }

        QString client_name = index.data(ChatModel::ClientNameRole).toString();
        QRect client_name_rect = rect;
        client_name_rect.setLeft(rect.left() + 40);
        client_name_rect.setTop(rect.top() + 5);
        painter->setFont(font);
        painter->drawText(client_name_rect, Qt::AlignLeft, client_name);

        QString last_message = index.data(ChatModel::LastMessageRole).toString();
        QRect last_message_rect = rect;
        last_message_rect.setLeft(rect.left() + 40);
        last_message_rect.setTop(rect.top() + 25);
        font.setPointSize(8);
        painter->setFont(font);
        painter->drawText(last_message_rect, Qt::AlignLeft | Qt::TextWordWrap, last_message);

        int unread_messages = index.data(ChatModel::UnreadMessagesRole).toInt();
        if (unread_messages)
        {
            QString unread_messages_text = QString::number(unread_messages);
            QRect unread_messages_rect = QRect(rect.right() - 40, rect.top() + 5, 20, 20);
            painter->setBrush(QColorConstants::Svg::lightskyblue);
            painter->setPen(Qt::NoPen);
            painter->drawEllipse(unread_messages_rect);

            painter->setPen(Qt::black);
            painter->setFont(font);
            painter->drawText(unread_messages_rect, Qt::AlignCenter, unread_messages_text);
        }

        painter->setPen(QPen(Qt::gray, 1));
        painter->drawLine(rect.bottomLeft(), rect.bottomRight());

        painter->restore();
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override { return QSize(option.rect.width(), 50); }
};