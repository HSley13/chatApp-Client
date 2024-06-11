#include "chat_line.h"
#include <QTime>

chat_line::chat_line(QWidget *parent)
    : QMainWindow(parent)
{
    central_widget = new QWidget();
    setCentralWidget(central_widget);

    message_layout = new QVBoxLayout();

    VBOX = new QVBoxLayout(central_widget);
}

QStringList chat_line::split_message(const QString &message, const int &chunk_size)
{
    QStringList chunks;
    int length = message.length();

    for (int i = 0; i < length; i += chunk_size)
        chunks.append(message.mid(i, chunk_size));

    return chunks;
}

void chat_line::set_message(const QString &message, bool is_mine, const QString &time)
{
    QLayoutItem *child;
    while ((child = VBOX->takeAt(0)) != 0)
    {
        delete child->widget();
        delete child;
    }

    QStringList lines = split_message(message, 100);

    Qt::Alignment alignment = is_mine ? Qt::AlignRight : Qt::AlignLeft;

    for (const QString &line : lines)
    {
        QLabel *msg_label = new QLabel(line, this);

        msg_label->setWordWrap(true);
        msg_label->setAlignment(alignment);

        VBOX->addWidget(msg_label);
    }

    QLabel *time_label = new QLabel(time, this);
    time_label->setAlignment(alignment);

    VBOX->addWidget(time_label);
}

void chat_line::set_group_message(const QString &message, const QString &sender, bool is_mine, const QString &date_time)
{
    QLayoutItem *child;
    while ((child = VBOX->takeAt(0)) != 0)
    {
        delete child->widget();
        delete child;
    }

    QLabel *msg_label = new QLabel(QString("%1: %2").arg(sender, message), this);
    QLabel *time_label = new QLabel(date_time, this);

    Qt::Alignment alignment = is_mine ? Qt::AlignRight : Qt::AlignLeft;

    msg_label->setAlignment(alignment);
    time_label->setAlignment(alignment);

    VBOX->addWidget(msg_label);
    VBOX->addWidget(time_label);
}
