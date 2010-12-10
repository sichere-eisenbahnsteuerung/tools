#include "consolewidget.h"
#include "ui_consolewidget.h"

ConsoleWidget::ConsoleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConsoleWidget)
{
    ui->setupUi(this);

    initialized = false;
    running = false;

    connect(ui->send_button, SIGNAL(clicked()), this, SLOT(send_button_pressed()));
}

ConsoleWidget::~ConsoleWidget()
{
    delete ui;
}


void ConsoleWidget::paintEvent( QPaintEvent *event )
{
    setMinimumSize(parentWidget()->size());
    setMaximumSize(parentWidget()->size());
    event->accept();
}

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
 /*   case TYPE_STATUS:
        outMsg.append("Status:\t");
        ui->console_text->setTextColor(QColor("lime"));
        break;*/
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

    return SUCCESS;
}


void ConsoleWidget::send_button_pressed()
{
    if(running)
    {
        emit outgoing_command(ui->command_line->text());
        ui->command_line->clear();
    }
}
