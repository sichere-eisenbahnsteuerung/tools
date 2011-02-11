#include "serialinterface.h"

/** @brief  Konstruktor der Klasse.
  *
  *         Wird beim Erstellen eines Serial-Interface Objekts aufgerufen. Initialisiert die
  *         Klassenweit verfügbaren Status-Variablen.
  */
SerialInterface::SerialInterface(QObject *parent) :
    QThread(parent)
{
    initialized = false;
    stopping = false;
    running = false;
    configured = false;
}


/** @brief  Übergibt eine Konfigurationsstruktur an das Interface.
  *
  *         Die Struktur "SerialConfig" enthält alle Parameter um einen COM-Port zu öffnen.
  *         Das Interface lässt sich nur dann konfigurieren wenn es nicht läuft.
  *
  * @param      config      Konfigurationsdatei
  *
  * @return     SUCCESS     Konfiguration erfolgreich.
  * @return     FAILURE     Konfiguration war nicht möglich.
  */
int SerialInterface::configure_serial_interface(SerialConfig config)
{
    emit status_message("Konfiguriere...", SOURCE_SERIAL, TYPE_STATUS_LVL_2);
    if(!running && !stopping)
    {
        configuration = config;
        initialized = false;
        configured = true;
        emit status_message("Konfiguration erfolgreich aktualisiert!", SOURCE_SERIAL, TYPE_STATUS_LVL_1);
        return SUCCESS;
    }
    else
    {
        emit status_message("Konfiguration fehlgeschlagen(Schnittstelle läuft bereits)", SOURCE_SERIAL, TYPE_ERROR);
        return FAILURE;
    }

}



/** @brief  Initialisiert die serielle Schnittstelle.
  *
  *         Es wird dafür die übergebene Kofiguration verwendet um den COM-Port zu öffnen
  *         und die übergebenen Parameter zu setzen. Eine initialisierung kann nur durchgeführt
  *         werden wenn die Schnittstelle nicht läuft und eine Konfiguration übergeben wurde.
  *
  * @return     SUCCESS     Schnittstelle wurde erfolgreich initialisiert.
  * @return     FAILURE     Fehler bei der Initialisierung.
  */
int SerialInterface::initialize_serial_interface()
{
    DCB comsettings;
    COMMTIMEOUTS timeoutsettings;

    if(!running && !stopping && configured && !initialized)
    {

        emit status_message("Initialisiere Serielle Verbindung...", SOURCE_SERIAL, TYPE_STATUS_LVL_2);

        WCHAR portName[5] = {'C','O','M','x','\0'};
        portName[3] = tr("%1").arg(configuration.comport).at(0).toAscii();

        emit status_message(tr("Öffne COM-Port %1").arg(configuration.comport), SOURCE_SERIAL, TYPE_STATUS_LVL_3);

        porthandle = CreateFile(&portName[0],
                               GENERIC_WRITE | GENERIC_READ,
                               0,
                               0,
                               OPEN_EXISTING,
                               0,
                               0);

        if(porthandle == NULL)
        {
            emit status_message("COM-Port kann nicht geöffnet werden.", SOURCE_SERIAL, TYPE_ERROR);
            return FAILURE;
        }

        if(GetCommState(porthandle,&comsettings))
        {
            comsettings.BaudRate = configuration.baudrate;

            if(configuration.stopbits == 1) comsettings.StopBits = ONESTOPBIT;
            else if(configuration.stopbits == 1.5) comsettings.StopBits = ONE5STOPBITS;
            else if(configuration.stopbits == 2) comsettings.StopBits = TWOSTOPBITS;
            else
            {
                emit status_message("COM-Port Einstellungen(Stop-Bits) enthält ungültigen Wert!", SOURCE_SERIAL, TYPE_ERROR);
                return FAILURE;
            }

            comsettings.ByteSize = configuration.databits;

            if(configuration.parity == PARITY_NO_PARITY) {comsettings.Parity = NOPARITY; comsettings.fParity  = FALSE;}
            else if(configuration.parity == PARITY_EVEN_PARITY) {comsettings.Parity = EVENPARITY; comsettings.fParity  = TRUE;}
            else if(configuration.parity == PARITY_ODD_PARITY) {comsettings.Parity = ODDPARITY; comsettings.fParity  = TRUE;}
            else if(configuration.parity == PARITY_MARK_PARITY) {comsettings.Parity = MARKPARITY; comsettings.fParity  = TRUE;}
            else if(configuration.parity == PARITY_SPACE_PARITY) {comsettings.Parity = SPACEPARITY; comsettings.fParity  = TRUE;}
            else
            {
                emit status_message("COM-Port Einstellungen(Parität) enthält ungültigen Wert!", SOURCE_SERIAL, TYPE_ERROR);
                return FAILURE;
            }

            if(configuration.flowcontrol == FLOW_NO_CONTROL)
            {
                comsettings.fOutX = false;
                comsettings.fInX = false;
                comsettings.fOutxCtsFlow = false;
                comsettings.fRtsControl = RTS_CONTROL_DISABLE;
            }
            else if (configuration.flowcontrol == FLOW_XON_XOFF)
            {
                comsettings.fOutX = true;
                comsettings.fInX = true;
                comsettings.fOutxCtsFlow = false;
                comsettings.fRtsControl = RTS_CONTROL_DISABLE;
            }
            else if(configuration.flowcontrol == FLOW_HARDWARE)
            {
                comsettings.fOutX = false;
                comsettings.fInX = false;
                comsettings.fOutxCtsFlow = true;
                comsettings.fRtsControl = RTS_CONTROL_HANDSHAKE;
            }
            else
            {
                emit status_message("COM-Port Einstellungen(Flow) enthält ungültigen Wert!", SOURCE_SERIAL, TYPE_ERROR);
                return FAILURE;
            }


            if(!SetCommState(porthandle, &comsettings))
            {
                emit status_message("Konnte COM-Port Einstellungen nicht schreiben!", SOURCE_SERIAL, TYPE_ERROR);
                return FAILURE;
            }
        }
        else
        {
            emit status_message("Konnte COM-Port Einstellungen nicht lesen!", SOURCE_SERIAL, TYPE_ERROR);
            return FAILURE;
        }

        if(GetCommTimeouts(porthandle, &timeoutsettings))
        {
            timeoutsettings.ReadIntervalTimeout = configuration.read_interval_timeout;
            timeoutsettings.ReadTotalTimeoutConstant = configuration.read_total_timeout_constant;
            timeoutsettings.ReadTotalTimeoutMultiplier = configuration.read_total_timeout_multiplier;
            timeoutsettings.WriteTotalTimeoutConstant = configuration.write_total_timeout_constant;
            timeoutsettings.WriteTotalTimeoutMultiplier = configuration.write_total_timeout_multiplier;

            if(!SetCommTimeouts(porthandle,&timeoutsettings))
            {
                emit status_message("Konnte COM-Port Timings nicht schreiben!", SOURCE_SERIAL, TYPE_ERROR);
                return FAILURE;
            }
        }
        else
        {
            emit status_message("Konnte COM-Port Timings nicht lesen!", SOURCE_SERIAL, TYPE_ERROR);
            return FAILURE;
        }


        emit status_message("Serielle Verbindung geöffnet!", SOURCE_SERIAL, TYPE_STATUS_LVL_1);
        initialized = true;
        return SUCCESS;
    }
    else
    {
        if(initialized)
        {
            emit status_message("Initialisierung nicht möglich: Ist bereits Initialisiert!", SOURCE_SERIAL, TYPE_ERROR);
        }
        if(!configured)
        {
            emit status_message("Initialisierung nicht möglich: Keine Konfiguration übergeben!", SOURCE_SERIAL, TYPE_ERROR);
        }
        if(stopping || running)
        {
            emit status_message("Initialisierung nicht möglich: Interface läuft bereits!", SOURCE_SERIAL, TYPE_ERROR);
        }

        return FAILURE;
    }
}


/** @brief  Gibt zurück ob das Modul beendet ist.
  */
bool SerialInterface::is_terminated()
{
    if(!initialized && !running && !stopping)
    {
        return true;
    }
    else
    {
        return false;
    }
}


/** @brief  Hauptschleife des Threads. Empfängt Daten und gibt sie zurück.
  *
  *         Empfängt Daten über den COM-Port ohne diese zu interpretieren. Daten werden je
  *         nach gewählten Timeouts in unregelmäßigen Blöcken gelesen und per SIGNAL weitergeleitet.
  */
void SerialInterface::run()
{
    char in_buffer[300];
    //char out_buffer[300];
    //DWORD bytes_sent;
    DWORD bytes_read;

    running = true;

    while(!stopping)
    {
        //bytes_sent = 0x00;
        bytes_read = 0x00;

        for(int i = 0; i < 300; i++)
        {
            in_buffer[i] = 0x00;
            //out_buffer[i] = 0x00;
        }

 /*       if(!out_queue.isEmpty())
        {
            for(int i = 0; i < out_queue.first().length(); i++)
            {
                out_buffer[i] = out_queue.first().at(i);
            }

            WriteFile(porthandle,&out_buffer[0],out_queue.first().length(),&bytes_sent,NULL);

            if(bytes_sent == out_queue.first().length())
            {
                out_queue.removeFirst();
                //emit status, etc.
            }
        } */

        ReadFile(porthandle,            // Handle
                    &in_buffer[0],      // Incoming data
                    8,                  // Number of bytes to read
                    &bytes_read,        // Number of bytes read
                    0);                 // Overlapped

        if(bytes_read != 0x00)
        {
            QByteArray data;
            for(unsigned int i = 0; i < bytes_read; i++)
            {
                data.append(in_buffer[i]);
            }
            emit incoming_bytes(data);
        }
    }

    if(CloseHandle(porthandle) == 0)
    {
        emit status_message("Kann COM-Port nicht freigeben!", SOURCE_SERIAL, TYPE_ERROR);
    }
    else
    {
        initialized = false;
    }

    emit status_message("Gestoppt", SOURCE_SERIAL, TYPE_STATUS_LVL_1);
    running = false;
    stopping = false;
}



/* @brief  Schnittstelle zum Senden von Daten.
  *
  *         Reiht die Nachricht zum Versand ein
  *
  * @return     SUCCESS     Nachricht erfolgreich erstellt und eingereiht.
  *             FAILURE     Fehler beim einreihen der Nachricht.
  *
int SerialInterface::send_message(QByteArray message)
{
    if(!running || stopping)
    {
        emit status_message("Kann Nachricht nichtKeine aktive Verbindung", SOURCE_SERIAL, TYPE_ERROR);
        return FAILURE;
    }
    else
    {
        emit status_message("Nachricht zum senden eingereiht", SOURCE_SERIAL, TYPE_STATUS_LVL_3);
        out_queue.append(message);
        return SUCCESS;

    }
}
*/


/** @brief      Startet die serielle Schnittstelle.
  *
  *             Ein Start ist nur möglich wenn die Schnittstelle initialisiert ist und nicht läuft.
  *
  * @return     SUCCESS     Start erfolgreich.
  * @return     FAILURE     Start war nicht möglich.
  */
int SerialInterface::start_serial_interface()
{
    if(initialized && !running && !stopping)
    {
        this->start();
        emit status_message("Gestartet", SOURCE_SERIAL, TYPE_STATUS_LVL_1);
        return SUCCESS;
    }
    else
    {
        emit status_message("Konnte nicht starten!", SOURCE_SERIAL, TYPE_ERROR);
        return FAILURE;
    }
}



/** @brief  Funktion zum Anhalten der seriellen Schnittstelle.
  *
  *         Es wird ein Flag(stopping) gesetzt welches dem Thread signalisiert, dass
  *         er anhalten soll. Es können u.U noch Nachrichten empfangen werden bevor
  *         der Thread gestoppt ist.
  *
  * @return     SUCCESS     Stop-Flag wurde erfolgreich gesetzt.
  * @return     FAILURE     Fehler beim Setzen des Stop-Flags.
  */
int SerialInterface::stop_serial_interface()
{
    if(running)
    {
        if(stopping)
        {
            emit status_message("Serielle Verbindung wird bereits geschlossen...", SOURCE_SERIAL, TYPE_ERROR);
            return FAILURE;
        }
        else
        {
            stopping = true;
            emit status_message("Serielle Verbindung wird geschlossen...", SOURCE_SERIAL, TYPE_STATUS_LVL_2);
            return SUCCESS;
        }
    }
    else
    {
        emit status_message("Keine aktive Verbindung!", SOURCE_SERIAL, TYPE_ERROR);
        return FAILURE;
    }
}
