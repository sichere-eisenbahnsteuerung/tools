#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "defines.h"
#include "includes.h"

#include "filelogging.h"
#include "serialinterface.h"
#include "guiwidget.h"
#include "consolewidget.h"
#include "interpreter.h"
#include "configurationwidget.h"

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    AppConfig configuration;
    SerialInterface *serialinterface;
    GuiWidget *guiwidget;
    ConsoleWidget *consolewidget;
    FileLogging *filelog;
    Interpreter *interpreter;
    ConfigurationWidget *configwidget;
    bool ready_to_exit;

    int initialize_application();
    int stop_application();

private slots:
    void aktion_verbinden();
    void aktion_trennen();
    void aktion_beenden();

    void console_receive_data();
    void interpreter_receive_incoming_data();
    void interpreter_receive_outgoing_data(QByteArray data);
    void serial_receive_data(QByteArray data);
    void status_message_hub(QString message, int source, int type);

#ifdef DEBUG_MODE
    void debug_commandline();
#endif

protected:
    void closeEvent(QCloseEvent *event);

};

#endif // MAINWINDOW_H
