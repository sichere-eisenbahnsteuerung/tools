#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QWidget>
#include "defines.h"
#include "includes.h"

namespace Ui {
    class ConsoleWidget;
}

class ConsoleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConsoleWidget(QWidget *parent = 0);
    ~ConsoleWidget();
    int start_console();
    int stop_console();
    int initialize_console();
    int configure_console(ConsoleConfig config);

    int data_output(QString message, int source, int type);

private:
    Ui::ConsoleWidget *ui;
    bool initialized;
    bool running;
    ConsoleConfig configuration;

private slots:
    void send_button_pressed();

protected:
    void paintEvent( QPaintEvent *event );

signals:
    void outgoing_command(QString command);
    void status_message(QString message, int source, int type);
};

#endif // CONSOLEWIDGET_H
