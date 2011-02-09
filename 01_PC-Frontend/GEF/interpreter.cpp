#include "interpreter.h"

/** @brief  Konstruktor der Klasse
  *
  * @todo   Implementieren
  */
Interpreter::Interpreter(QObject *parent) :
    QObject(parent)
{
    initialized = false;
    running = false;
    //in_queue.clear();
}


/** @brief  Setzt eine Neue Konfiguration für das Modul
  *
  * @param  config  Neue Konfiguration
  *
  * @todo   Implementieren
  */
int Interpreter::configure_interpreter(InterpreterConfig config)
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


/** @brief  Initialisiert das Modul mit der aktuellen Konfiguration
  *
  * @todo   Implementieren
  */
int Interpreter::initialize_interpreter()
{
    if(running)
    {
        return FAILURE;
    }
    else
    {
        //in_queue.clear();
        initialized = true;
        return SUCCESS;
    }
}


/** @brief  Gibt zurück ob das Modul beendet ist.
  */
bool Interpreter::is_terminated()
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


/** @brief  Startet das Modul
  *
  * @todo   Implementieren
  */
int Interpreter::start_interpreter()
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


/** @brief  Beendet das Modul kontrolliert.
  *
  * @todo   Implementieren
  */
int Interpreter::stop_interpreter()
{
    if(running)
    {
        running = false;
        //in_queue.clear();
        return SUCCESS;
    }
    else
    {
        return FAILURE;
    }
}


/** @brief  Sammelt eingehende Rohdaten und stößt interpretation an
  *
  * @param  data    Rohdaten die zu interpretieren sind.
  *
  * @todo   Implementieren
  */
int Interpreter::add_incoming_data(QByteArray data)
{
    //in_queue.append(data);

    //TODO
    return SUCCESS;
}


/** @brief  Kodiert eine Nachricht zum senden über die Serielle Schnittstelle
  *
  * @param  message     Zu kodierende Nachricht
  *
  * @todo   Implementieren
  */
int Interpreter::encode_outgoing_message(QString message)
{
    //TODO
    return SUCCESS;
}
