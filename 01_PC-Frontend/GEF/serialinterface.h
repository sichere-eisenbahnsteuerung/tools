#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

#include "defines.h"
#include "includes.h"

/** @brief  Regelt den gesamten Rohdatenverkehr über den COM-Port
  *
  *         Die Klasse verfügt über kein "Wissen" über die Dateninhalte sondern empfängt
  *         lediglich die Rohdaten und gibt diese zurück.
  */
class SerialInterface : public QThread
{
    Q_OBJECT
public:
    explicit SerialInterface(QObject *parent = 0);

    int configure_serial_interface(SerialConfig config);
    int initialize_serial_interface();
    int stop_serial_interface();
    int start_serial_interface();

    bool is_terminated();

    //int send_message(QByteArray message);

private:
    SerialConfig configuration;
    bool initialized;
    bool stopping;
    bool running;
    bool configured;

    //QList<QByteArray> out_queue;

    HANDLE porthandle;

protected:
    void run();

signals:
    void incoming_bytes(QByteArray data);
    void status_message(QString message, int source, int type);
};

#endif // SERIALINTERFACE_H
