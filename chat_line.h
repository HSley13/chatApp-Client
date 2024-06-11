#pragma once

#include <QtWidgets>
#include <QtCore>
class chat_line : public QMainWindow
{
    Q_OBJECT
public:
    chat_line(QWidget *parent = nullptr);

    QStringList split_message(const QString &message, const int &chunk_size);

    void set_message(const QString &message, bool true_or_false, const QString &date_time);

    void set_group_message(const QString &message, const QString &sender, bool is_mine, const QString &date_time);

private:
    QWidget *central_widget;

    QVBoxLayout *VBOX;
    QVBoxLayout *message_layout;

    QLabel *message;

    QLabel *time;
};