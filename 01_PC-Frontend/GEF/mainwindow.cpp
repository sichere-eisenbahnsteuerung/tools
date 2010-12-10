#include "mainwindow.h"
#include "ui_mainwindow.h"

/** @brief  Konstruktor der Klasse.
  *
  *
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

    filelog = new FileLogging(this);
    serialinterface = new SerialInterface(this);
    guiwidget = new GuiWidget(ui->gui_widget);   
    consolewidget = new ConsoleWidget(ui->console_widget);
    interpreter = new Interpreter(this);

    configwidget = new ConfigurationWidget(ui->config_widget);
    configwidget->configure_config_widget("GEF.ini");

    connect(ui->actionBeenden, SIGNAL(triggered()), this, SLOT(aktion_beenden()));
    connect(ui->actionTrennen, SIGNAL(triggered()), this, SLOT(aktion_trennen()));
    connect(ui->actionVerbinden, SIGNAL(triggered()), this, SLOT(aktion_verbinden()));

    ready_to_exit = true;

    initialize_application();
}

/** @brief  Destruktor der Klasse.
  *
  *
  */
MainWindow::~MainWindow()
{
    delete ui;
}


/** @brief  Initialisiert alle Module.
  *
  * @return SUCCESS Alle Module erfolgreich initialisiert.
  *         FAILURE Fehler bei mindestens einer Initialisierung.
  *
  * @todo   Fehler abfangen
  */
int MainWindow::initialize_application()
{
    this->configuration = configwidget->get_config_app();

    guiwidget->configure_gui(configwidget->get_config_gui());
    guiwidget->initialize_gui();
    guiwidget->start_gui();

    consolewidget->configure_console(configwidget->get_config_console());
    consolewidget->initialize_console();
    consolewidget->start_console();

    filelog->configure_log(configwidget->get_config_log());
    filelog->initialize_log();
    filelog->start_log();

    interpreter->configure_interpreter(configwidget->get_config_interpreter());
    interpreter->initialize_interpreter();
    interpreter->start_interpreter();

    return SUCCESS;
}


/** @brief  Stoppt alle Module.
  *
  * @return SUCCESS Alle Module erfolgreich gestoppt.
  *         FAILURE Fehler bei mindestens einem Modul.
  *
  * @todo   Implementieren
  */
int MainWindow::stop_application()
{

    return SUCCESS;
}




//////////////////////////////////////SLOTS///////////////////////////////////////////////////

/** @brief  Initialisiert die Serielle Schnittstelle mit den Einstellungen aus der .ini Datei
  *
  *         - Lädt die Werte aus der .ini Datei
  *         - Erstellt ein neues SerialInterface Objekt mit Hilfe des lokalen Pointers
  *         - Übergibt dabei die Konfiguration
  *         - Versucht die Schnittstelle zu initialisieren.
  *             -Wenn Erfolgreich wird der Thread gestartet.
  */
void MainWindow::aktion_verbinden()
{
    serialinterface->configure_serial_interface(configwidget->get_config_serial());
    serialinterface->initialize_serial_interface();
    serialinterface->start_serial_interface();
}



/** @brief  Behandlung des "Trennen" Buttons
  *
  */
void MainWindow::aktion_trennen()
{

}



/** @brief  Beendet das Programm kontrolliert
  *
  */
void MainWindow::aktion_beenden()
{

}


/** @brief  Sammelt alle Status nachrichten der Module
  *
  * @param  message     Inhalt der Statusnachricht
  * @param  source      Sendendes Modul
  * @param  type        Nachrichtentyp
  *
  * @todo   Implementieren
  */
void MainWindow::status_message_hub(QString message, int source, int type)
{
    //je nach verbose an konsole senden

    //je nach setting an filelog senden
}


/** @brief  Empfängt die Konsoleneingaben
  *
  * @todo   Implementieren
  */
void MainWindow::console_receive_data()
{
    //daten an interpreter senden

    //je nach verbose an konsole senden
}


/** @brief  Empfängt die vom Interpreter fertig dekodierten Nachrichten
  *
  * @todo   Implementieren
  */
void MainWindow::interpreter_receive_incoming_data()
{
    //an konsole senden
    //an gui senden
    //an filelog senden
}


/** @brief  Empfängt die vom Interpreter fertig kodierten Nachrichten.
  *
  * @param  data    Telegramm, bereit zum senden.
  *
  * @todo   Implementieren
  */
void MainWindow::interpreter_receive_outgoing_data(QByteArray data)
{
    //an serialinterface senden
    //an konsole senden
    //an filelog senden
}


/** @brief  Sammelt alle Rohdaten der seriellen Schnittstelle
  *
  * @param  data    Rohdaten von der seriellen Schnittstelle
  *
  * @todo   Implementieren
  */
void MainWindow::serial_receive_data(QByteArray data)
{
    //an interpreter senden
    //an konsole senden
    //an filelog senden
}



/////////////////////////////PROTECTED////////////////////////////////////

/** @brief  Fängt close events des Hauptfensters ab um kontrolliert beenden zu können.
  *
  * @param  event   Enthält Daten zum Event
  *
  * @todo   Implementieren
  */
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(ready_to_exit)
    {
        event->accept();
    }
    else
    {
        this->aktion_beenden();
    }
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
