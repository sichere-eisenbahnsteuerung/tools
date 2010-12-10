#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QObject>
#include "defines.h"
#include "includes.h"

class Interpreter : public QObject
{
    Q_OBJECT
public:
    explicit Interpreter(QObject *parent = 0);

    int configure_interpreter(InterpreterConfig config);
    int initialize_interpreter();
    int start_interpreter();
    int stop_interpreter();

    int add_incoming_data(QByteArray data);
    int encode_outgoing_message(QString message);

private:
    bool initialized;
    bool running;

    QByteArray in_queue;

    InterpreterConfig configuration;

signals:
    void message_decoded(QList<RailEvent> events);
    void encoding_finished(QByteArray message);
    void status_message(QString message, int source, int type);


};

#endif // INTERPRETER_H
