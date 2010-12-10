#include "mainwindow.h"
#include "ui_mainwindow.h"

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

MainWindow::~MainWindow()
{
    delete ui;
}


//TODO: Fehler abfangen
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



/** @brief  Fordert die serielle Schnittstelle auf sich zu beenden
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



void MainWindow::status_message_hub(QString message, int source, int type)
{
    //je nach verbose an konsole senden

    //je nach setting an filelog senden
}



void MainWindow::console_receive_data()
{
    //daten an interpreter senden

    //je nach verbose an konsole senden
}



void MainWindow::interpreter_receive_incoming_data()
{
    //an konsole senden
    //an gui senden
    //an filelog senden
}



void MainWindow::interpreter_receive_outgoing_data(QByteArray data)
{
    //an serialinterface senden
    //an konsole senden
    //an filelog senden
}



void MainWindow::serial_receive_data(QByteArray data)
{
    //an interpreter senden
    //an konsole senden
    //an filelog senden
}



/////////////////////////////PROTECTED////////////////////////////////////

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
