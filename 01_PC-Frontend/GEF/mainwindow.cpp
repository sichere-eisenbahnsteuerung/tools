#include "mainwindow.h"
#include "ui_mainwindow.h"

/** @brief  Konstruktor der Klasse.
  *
  *         Wird automatisch bei Programmstart von der main-Funktion aufgerufen.
  *         Es werden alle Module als Objekte erstellt und die Anwendung initialisiert.
  */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

#ifndef DEBUG_MODE
    ui->debug_command_line->deleteLater();
    ui->debug_send_button->deleteLater();
#endif
#ifdef DEBUG_MODE
    connect(ui->debug_send_button, SIGNAL(clicked()), this, SLOT(debug_commandline()));
#endif

    filelog         =   new FileLogging(this);
    serialinterface =   new SerialInterface(this);
    guiwidget       =   new GuiWidget(ui->gui_widget);
    consolewidget   =   new ConsoleWidget(ui->console_widget);
    interpreter     =   new Interpreter(this);
    configwidget    =   new ConfigurationWidget(ui->config_widget);

    configwidget->configure_config_widget("GEF.ini");

    connect(this, SIGNAL(status_message(QString,int,int)), this, SLOT(status_message_hub(QString,int,int)));
    connect(filelog, SIGNAL(status_message(QString,int,int)), this, SLOT(status_message_hub(QString,int,int)));
    connect(serialinterface, SIGNAL(status_message(QString,int,int)), this, SLOT(status_message_hub(QString,int,int)));
    connect(guiwidget, SIGNAL(status_message(QString,int,int)), this, SLOT(status_message_hub(QString,int,int)));
    connect(consolewidget, SIGNAL(status_message(QString,int,int)), this, SLOT(status_message_hub(QString,int,int)));
    connect(interpreter, SIGNAL(status_message(QString,int,int)), this, SLOT(status_message_hub(QString,int,int)));

    connect(serialinterface, SIGNAL(incoming_bytes(QByteArray)), this, SLOT(serial_receive_data(QByteArray)));
    connect(interpreter, SIGNAL(message_decoded(QList<RailEvent>)), this, SLOT(interpreter_receive_incoming_data(QList<RailEvent>)));

    connect(ui->actionBeenden, SIGNAL(triggered()), this, SLOT(aktion_beenden()));
    connect(ui->actionTrennen, SIGNAL(triggered()), this, SLOT(aktion_trennen()));
    connect(ui->actionVerbinden, SIGNAL(triggered()), this, SLOT(aktion_verbinden()));

    terminating = false;

    initialize_application();
}


/** @brief  Initialisiert die Basismodule.
  *
  *         Es werden alle Module initialisiert und gestartet die f�r die Anzeige und Konfiguration
  *         notwendig sind.
  */
void MainWindow::initialize_application()
{
    this->configuration = configwidget->get_config_app();

    if(consolewidget->configure_console(configwidget->get_config_console()) == SUCCESS)
    {
        if(consolewidget->initialize_console() == SUCCESS)
        {
            consolewidget->start_console();
        }
    }

    if(guiwidget->configure_gui(configwidget->get_config_gui()) == SUCCESS)
    {
        if(guiwidget->initialize_gui() == SUCCESS)
        {
            guiwidget->start_gui();
        }
    }

    if(filelog->configure_log(configwidget->get_config_log()) == SUCCESS)
    {
        if(filelog->initialize_log() == SUCCESS)
        {
            filelog->start_log();
        }
    }
}




//////////////////////////////////////SLOTS///////////////////////////////////////////////////

/** @brief  Behandelt das Dr�cken des "Verbinden" Buttons.
  *
  *         Initialisiert und Startet den Interpreter und die Serielle Schnittstelle.
  */
void MainWindow::aktion_verbinden()
{
    if(terminating) return;

    bool interpreter_launched = false;

    if(interpreter->configure_interpreter(configwidget->get_config_interpreter()) == SUCCESS)
    {
        if(interpreter->initialize_interpreter() == SUCCESS)
        {
            if(interpreter->start_interpreter() == SUCCESS)
            {
                interpreter_launched = true;
            }
        }
    }

    if(interpreter_launched)
    {
        if(serialinterface->configure_serial_interface(configwidget->get_config_serial()) == SUCCESS)
        {
            if(serialinterface->initialize_serial_interface() == SUCCESS)
            {
                serialinterface->start_serial_interface();
            }
        }
    }
}



/** @brief  Behandelt das Dr�cken des "Trennen" Buttons.
  *
  *         Stoppt die Serielle Schnittstelle und den Interpreter.
  */
void MainWindow::aktion_trennen()
{
    if(terminating) return;

    if(serialinterface->stop_serial_interface() == FAILURE)
    {
        emit status_message("Fehler beim Trennen. System l�uft weiter!", SOURCE_SYSTEM, TYPE_ERROR);
    }
    else if(interpreter->stop_interpreter() == FAILURE)
    {
        emit status_message("Fehler im Interpreter. Interpreter l�uft weiter!", SOURCE_SYSTEM, TYPE_ERROR);
    }
}



/** @brief  Behandelt das Dr�cken des "Beenden" Buttons.
  *
  *         Alle Module erhalten den Befehl sich zu beenden. Best�tigen alle Module den Befehl
  *         wird mit "wait_for_termination" darauf gewartet, dass sich alle Module beenden.
  */
void MainWindow::aktion_beenden()
{
    if(terminating) return;

    if(serialinterface->stop_serial_interface() == FAILURE ||
       interpreter->stop_interpreter() == FAILURE ||
       filelog->stop_log() == FAILURE ||
       guiwidget->stop_gui() == FAILURE ||
       consolewidget->stop_console() == FAILURE)
    {
        terminating = false;
        emit status_message("Ein Modul l�sst sich nicht beenden!", SOURCE_SYSTEM, TYPE_ERROR);
    }
    else
    {
        terminating = true;
        this->wait_for_termination();
    }
}


/** @brief  Sammelt alle Statusnachrichten der Module.
  *
  *         Die Statusnachrichten werden je nach Konfiguration und Verbose-Einstellungen an die
  *         Konsole und/oder an das Filelog weitergeleitet.
  *
  * @param  message     Inhalt der Statusnachricht
  * @param  source      Sendendes Modul
  * @param  type        Nachrichtentyp
  *
  * @todo   Implementieren
  */
void MainWindow::status_message_hub(QString message, int source, int type)
{
    if(terminating) return;

    //je nach verbose-lvl an konsole senden

    //je nach setting an filelog senden
}



/** @brief  Empf�ngt die vom Interpreter fertig dekodierten Nachrichten.
  *
  *         Hat der Interpreter aus den Rohdaten ein Railevent extrahiert wird dieses
  *         an die Konsole, das Filelog und an das GuiWidget verteilt.
  *
  * @param  events    Liste aller neu erhaltenen Events
  *
  * @todo   Implementieren
  */
void MainWindow::interpreter_receive_incoming_data(QList<RailEvent> events)
{
    if(terminating) return;

    //an konsole senden
    //an filelog senden

    guiwidget->display_data(events);
}


/** @brief  Sammelt alle Rohdaten der seriellen Schnittstelle.
  *
  *         Die seriellen Rohdaten werden an den Interpreter und, je nach Verbose- und Logging Einstellungen,
  *         auch an die Konsole und das Filelog gesendet.
  *
  * @param  data    Rohdaten von der seriellen Schnittstelle
  *
  * @todo   Implementieren
  */
void MainWindow::serial_receive_data(QByteArray data)
{
    if(terminating) return;

    //ggf an konsole senden
    //ggf an filelog senden

    interpreter->add_incoming_data(data);
}


/** @brief  Wartet auf alle Module und beendet dann das Programm.
  *
  *         Pr�ft jedes Modul ob es beendet ist. Ist dies der Fall wird das Programm ebenfalls beendet.
  *         Ist mindestens ein Modul nicht beendet ruft sich die Funktion nach einer Sekunde erneut auf.
  */
void MainWindow::wait_for_termination()
{
    if(serialinterface->is_terminated() &&
       interpreter->is_terminated() &&
       consolewidget->is_terminated() &&
       filelog->is_terminated() &&
       guiwidget->is_terminated())
    {
        this->close();
    }
    else
    {
        QTimer::singleShot(1000, this, SLOT(wait_for_termination()));
    }
}



/////////////////////////////PROTECTED////////////////////////////////////

/** @brief  F�ngt close events des Hauptfensters ab um kontrolliert beenden zu k�nnen.
  *
  *         Wird das Hauptfenster �ber das �bliche "X" beendet. F�ngt diese �berdeckende Funktion
  *         den Befehl ab und ruft stattdessen die interne "aktion_beenden" auf. So k�nnen sich die
  *         einzelnen Module selbstst�ndig und kontrolliert beenden.
  *
  * @param  event   Enth�lt Daten zum Event. Werden nicht ben�tigt.
  */
void MainWindow::closeEvent(QCloseEvent *event)
{
    this->aktion_beenden();
}



///////////////////////////////DEBUG//////////////////////////////////////

#ifdef DEBUG_MODE

/**
  * @brief  Debug Funktion, Liest Debug-Konsole und leitet Nachricht an die Module weiter.
  */
void MainWindow::debug_commandline()
{
    QString command = ui->debug_command_line->text();
    if(!command.isEmpty())
    {
        QStringList commands = command.split(" ", QString::SkipEmptyParts);
        command = commands.at(0);
        commands.removeAt(0);

        if(command == "gui") guiwidget->debug_command(commands);

        ui->debug_command_line->clear();
    }
}

#endif
