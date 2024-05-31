#pragma once

#include <QtWidgets>
#include <QtCore>
class chat_line : public QMainWindow
{
    Q_OBJECT
public:
    chat_line(QWidget *parent = nullptr);

    void set_message(const QString &message, bool true_or_false, const QString &date_time);

private:
    QWidget *central_widget;

    QVBoxLayout *VBOX;
    QVBoxLayout *message_layout;

    QLabel *message;

    QLabel *time;
};