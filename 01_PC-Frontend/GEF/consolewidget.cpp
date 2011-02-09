#include "consolewidget.h"
#include "ui_consolewidget.h"


/** @brief  Konstruktor der Klasse
  *
  * @todo   Implementieren
  */
ConsoleWidget::ConsoleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConsoleWidget)
{
    ui->setupUi(this);

    initialized = false;
    running = false;

    connect(ui->send_button, SIGNAL(clicked()), this, SLOT(send_button_pressed()));
}

/** @brief  Destruktor der Klasse
  */
ConsoleWidget::~ConsoleWidget()
{
    delete ui;
}


/** @brief  Gibt zurück ob das Modul beendet ist.
  */
bool ConsoleWidget::is_terminated()
{
    if(!running)
    {
        return true;
    }
    else
    {
        return false;
    }
}


/** @brief  Startet das initialisierte Modul
  *
  * @todo   Implementieren
  */
int ConsoleWidget::start_console()
{
    if(initialized)
    {
        running = true;
        return SUCCESS;
    }
    else
    {
        return FAILURE;
    }
}
/** @brief  Stoppt das Modul
  *
  * @todo   Implementieren
  */


int ConsoleWidget::stop_console()
{
    if(running)
    {
        running = false;
        return SUCCESS;
    }
    else
    {
        return FAILURE;
    }
}


/** @brief  Initialisiert das Modul
  *
  * @todo   Implementieren
  */
int ConsoleWidget::initialize_console()
{
    if(running)
    {
        return FAILURE;
    }
    else
    {
        ui->console_text->clear();
        initialized = true;
        return SUCCESS;
    }
}


/** @brief  Konfiguriert das Modul
  *
  * @param  config  Neue Konfiguration
  *
  * @todo   Implementieren
  */
int ConsoleWidget::configure_console(ConsoleConfig config) //TODO: config noch ungenutzt
{
    if(running)
    {
        return FAILURE;
    }
    else
    {
        initialized = false;
        configuration = config;
        return SUCCESS;
    }
}


/** @brief  Schnittstelle zur visuellen Konsolenausgabe
  *
  *
  * @param      message     Enthät die auszugebende Nachricht
  * @param      source      Enthält die Quelle der Nachricht (siehe defines.h)
  * @param      type        enthält den Nachrichtentyp (siehe defines.h)
  *
  * @return     SUCCESS     Ausgabe war erfolgreich
  *             FAILURE     Ausgabe war nicht erfolgreich.
  */
int ConsoleWidget::data_output(QString message, int source, int type)
{
    /*
    if(!running)
    {
        return FAILURE;
    }

    QString outMsg("");
    outMsg.append(QTime::currentTime().toString("hh:mm:zzz"));
    outMsg.append("\t");

    switch(source)
    {
    case SOURCE_SERIAL:
        outMsg.append("Serial(IN):\t");
        break;
    case SOURCE_CONFIG:
        outMsg.append("Config:\t");
        break;
    case SOURCE_SYSTEM:
        outMsg.append("System:\t");
        break;
    case SOURCE_ARDUINO:
        outMsg.append("Arduino:\t");
        break;
    case SOURCE_LOGGING:
        outMsg.append("Dateilog:\t");
        break;
    default:
        outMsg.append("Unbekannte Quelle: ");
        break;
    }

    switch(type)
    {
    case TYPE_MESSAGE:
        outMsg.append("Message:\t");
        ui->console_text->setTextColor(QColor("white"));
        break;
    case TYPE_ERROR:
        outMsg.append("ERROR:\t");
        ui->console_text->setTextColor(QColor("red"));
        break;
    default:
        outMsg.append("Unbekannter Meldungstyp: ");
        ui->console_text->setTextColor(QColor("red"));
        break;
    }

    outMsg.append(message);
    ui->console_text->append(outMsg);

    if(ui->autoscroll_check->isChecked())
    {
        QTextCursor c = ui->console_text->textCursor();
        c.movePosition(QTextCursor::End);
        ui->console_text->setTextCursor(c);
    }
    */
    return SUCCESS;
}


///////////////////////////////SLOTS////////////////////////////////

/** @brief  Behandelt das Drücken des "Senden" Buttons
  *
  * @todo   Implementieren
  */
void ConsoleWidget::send_button_pressed()
{
    /*
    if(running)
    {
        emit outgoing_command(ui->command_line->text());
        ui->command_line->clear();
    }
    */
}

//////////////////////////PROTECTED///////////////////////////////////

/** @brief  Enthält Anweisungen zum neuzeichnen des Moduls
  *
  * @param  event   Enthät Daten über das event.
  *
  * @todo   Implementieren
  */
void ConsoleWidget::paintEvent( QPaintEvent *event )
{
    setMinimumSize(parentWidget()->size());
    setMaximumSize(parentWidget()->size());
    event->accept();
}
