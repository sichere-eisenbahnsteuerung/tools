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

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

private:
    Ui::MainWindow *ui;
    AppConfig configuration;

    SerialInterface     *serialinterface;
    GuiWidget           *guiwidget;
    ConsoleWidget       *consolewidget;
    FileLogging         *filelog;
    Interpreter         *interpreter;
    ConfigurationWidget *configwidget;

    bool terminating;

    void initialize_application();

private slots:
    void aktion_verbinden();
    void aktion_trennen();
    void aktion_beenden();

    void interpreter_receive_incoming_data(QList<RailEvent> events);
    void serial_receive_data(QByteArray data);
    void status_message_hub(QString message, int source, int type);

    void wait_for_termination();

#ifdef DEBUG_MODE
    void debug_commandline();
#endif

protected:
    void closeEvent(QCloseEvent *event);

signals:
    void status_message(QString message, int source, int type);

};

#endif // MAINWINDOW_H
