#include "filelogging.h"

/** @brief  Konstruktor der Klasse
  *
  * @todo   Implementieren
  */
FileLogging::FileLogging(QObject *parent) :
    QObject(parent)
{
    //this->configuration = config;
    running = false;
    initialized = false;
}


/** @brief  Fügt eine Nachricht dem Log an.
  *
  * @param  message     Inhalt der Statusnachricht
  * @param  source      Sendendes Modul
  * @param  type        Nachrichtentyp
  *
  * @todo   Implementieren
  */
int FileLogging::log_message(QString message, int source, int type)
{
    /*
    if(!running)
    {
        emit status_message("Kein aktives Dateilog", SOURCE_LOGGING, TYPE_ERROR);
        return FAILURE;
    }

    QString log_entry = "";
    log_entry.append(QDateTime::currentDateTime().toString("hh:mm:ss:zzz\t"));

    switch(source)
    {
    case SOURCE_SERIAL:
        log_entry.append("Serial:\t");
        break;
    case SOURCE_CONFIG:
        log_entry.append("Config:\t");
        break;
    case SOURCE_SYSTEM:
        log_entry.append("System:\t");
        break;
    case SOURCE_ARDUINO:
        log_entry.append("Arduino:\t");
        break;
    default:
        log_entry.append("Unbekannte Quelle: ");
        break;
    }

    switch(type)
    {
    case TYPE_MESSAGE:
        log_entry.append("Message:\t");
        break;
    case TYPE_ERROR:
        log_entry.append("ERROR:\t");
        break;
    default:
        log_entry.append("Unbekannter Meldungstyp: ");
        break;
    }

    log_entry.append(message);
    log_entry.append("\r\n");

    QTextStream dstream(logfile);
    dstream << log_entry;

    */

    return SUCCESS;
}



/** @brief  Startet das Modul
  *
  * @todo   Implementieren
  */
int FileLogging::start_log()
{
    /*
    if(running)
    {
        emit status_message("Logging läuft bereits", SOURCE_LOGGING, TYPE_ERROR);
        return FAILURE;
    }
    emit status_message("Starte Dateilog", SOURCE_LOGGING, TYPE_STATUS_LVL_2);

    QString filename = "";
    filename.append(configuration.filename);

    if(configuration.timestamp_filename)
    {
        filename.append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss"));
    }

    filename.append(".txt");

    logfile = new QFile(filename);
    if(configuration.append_to_file)
    {
        if(logfile->open(QIODevice::WriteOnly|QIODevice::Append))
        {
            emit status_message("Dateilog geöffnet", SOURCE_LOGGING, TYPE_STATUS_LVL_3);
            running = true;
            return SUCCESS;
        }
        else
        {
            emit status_message("Datei konnte nicht geöffnet werden!", SOURCE_LOGGING, TYPE_ERROR);
            return FAILURE;
        }
    }
    else
    {
        if(logfile->open(QIODevice::WriteOnly|QIODevice::Truncate))
        {
            emit status_message("Dateilog geöffnet", SOURCE_LOGGING, TYPE_STATUS_LVL_3);
            running = true;
            return SUCCESS;
        }
        else
        {
            emit status_message("Datei konnte nicht geöffnet werden!", SOURCE_LOGGING, TYPE_ERROR);
            return FAILURE;
        }
    }

    */
    return SUCCESS; //dummy
}



/** @brief  Stoppt das Modul kontrolliert
  *
  * @todo   Implementieren
  */
int FileLogging::stop_log()
{
    /*
    if(!running)
    {
        emit status_message("Kein aktives Dateilog", SOURCE_LOGGING, TYPE_ERROR);
        return FAILURE;
    }

    emit status_message("Stoppe Dateilog", SOURCE_LOGGING, TYPE_STATUS_LVL_2);
    if(logfile->flush())
    {
        logfile->close();
        return SUCCESS;
    }
    else
    {
        emit status_message("Datei konnte nicht geschlossen werden (Flush fehlgeschlagen)!", SOURCE_LOGGING, TYPE_ERROR);
        return FAILURE;
    }
    */
    return SUCCESS; //dummy
}


/** @brief  Initialisiert das Modul mit der aktuellen Konfiguration
  *
  * @todo   Implementieren
  */
int FileLogging::initialize_log()
{
    if(running)
    {
        return FAILURE;
    }
    else
    {
        //TODO
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
int FileLogging::configure_log(LogConfig config)
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
