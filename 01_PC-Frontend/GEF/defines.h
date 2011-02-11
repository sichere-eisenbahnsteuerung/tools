#ifndef DEFINES_H
#define DEFINES_H

#include "includes.h"

//Min Debug-Modus verfügt die GUI über eine Debug-Konsole zur Eingabe von Befehlen
#define DEBUG_MODE

//Funktionsrückgabewerte
#define SUCCESS             0
#define FAILURE             -1

////////////////////Status Nachrichten//////////////////////////

//Zur Verwendung in Status-Messages um die Herkunft einer Nachricht anzugeben
#define SOURCE_SERIAL              1
#define SOURCE_GUI                 2
#define SOURCE_CONFIG              3
#define SOURCE_SYSTEM              4
#define SOURCE_INTERPRETER         5
#define SOURCE_LOGGING             6
#define SOURCE_CONSOLE             7

//Verschiedene Typen von Statusnachrichten
#define TYPE_ERROR               -1
#define TYPE_STATUS_LVL_1        1  //bestätigungen
#define TYPE_STATUS_LVL_2        2  //grobe abläufe
#define TYPE_STATUS_LVL_3        3  //jedes interne detail
#define TYPE_STATUS_LVL_4        4  //details von unterfunktionen


////////////////////Serielle Schnittstelle////////////////////////

//Paritäten
#define PARITY_NO_PARITY        0
#define PARITY_EVEN_PARITY      1
#define PARITY_ODD_PARITY       2
#define PARITY_MARK_PARITY      3
#define PARITY_SPACE_PARITY     4

//Flusssteuerung
#define FLOW_NO_CONTROL         1
#define FLOW_XON_XOFF           2
#define FLOW_HARDWARE           3

//////////////////////Schienennetz/////////////////////////////////

//Objekte im Schiennetz
#define RAIL_SENSOR             1
#define RAIL_COUPLER            2
#define RAIL_SWITCH             3
#define RAIL_TRAIN              4

//Events im Schienennetz
#define RAIL_EVENT_ACTIVATED    1
#define RAIL_EVENT_SPEED        2



//////////////////////Strukturen/////////////////////////////////

/** @brief  Beinhaltet alle Daten eines Ereignisses.
  *
  *         Ein Railevent beschreibt ein vorkommen im Schienennetz. Beispielsweise
  *         das aktivieren eines Sensors.
  */
typedef struct RailEvent
{
    int object_type;
    int object_number;
    int event;
    int data;
}RailEvent;


/** @brief  Beinhaltet die Konfiguration der seriellen Schnittstelle.
  *
  *         Alle benötigten Konfigurationsdaten werden in diesem Strukt gespeichert.
  */
typedef struct SerialConfig
{
    unsigned int comport;
    unsigned int baudrate;
    unsigned int databits;
    unsigned int parity;
    double stopbits;
    unsigned int flowcontrol;

    unsigned int read_interval_timeout;
    unsigned int read_total_timeout_multiplier;
    unsigned int read_total_timeout_constant;
    unsigned int write_total_timeout_multiplier;
    unsigned int write_total_timeout_constant;
}SerialConfig;


/** @brief  Beinhaltet die Konfiguration des Dateilogs.
  *
  *         Alle benötigten Konfigurationsdaten werden in diesem Strukt gespeichert.
  *
  * @todo   Den Klassenbedürfnissen anpassen
  */
typedef struct LogConfig
{
    QString filename;
    bool append_to_file;
    bool timestamp_filename;
}LogConfig;


/** @brief  Beinhaltet die Konfiguration der Konsole.
  *
  *         Alle benötigten Konfigurationsdaten werden in diesem Strukt gespeichert.
  *
  * @todo   Den Klassenbedürfnissen anpassen
  */
typedef struct ConsoleConfig
{
    int font_size;
}ConsoleConfig;


/** @brief  Beinhaltet die Konfiguration des Interpreters.
  *
  *         Alle benötigten Konfigurationsdaten werden in diesem Strukt gespeichert.
  *
  * @todo   Den Klassenbedürfnissen anpassen
  */
typedef struct InterpreterConfig
{
    int testvalue;
}InterpreterConfig;


/** @brief  Beinhaltet die Konfiguration der GUI.
  *
  *         Alle benötigten Konfigurationsdaten werden in diesem Strukt gespeichert.
  */
typedef struct GuiConfig
{
    int frames_per_second;
    int active_time;
    int draw_sensor_size;
    int draw_switch_size;
    int draw_coupler_size;
}GuiConfig;


/** @brief  Beinhaltet die Konfiguration des MainWindow.
  *
  *         Alle benötigten Konfigurationsdaten werden in diesem Strukt gespeichert.
  */
typedef struct AppConfig
{
    int testvalue;
}AppConfig;

#endif // DEFINES_H
