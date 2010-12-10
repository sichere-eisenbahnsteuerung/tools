#include "interpreter.h"

Interpreter::Interpreter(QObject *parent) :
    QObject(parent)
{
    initialized = false;
    running = false;
    in_queue.clear();
}

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

int Interpreter::initialize_interpreter()
{
    if(running)
    {
        return FAILURE;
    }
    else
    {
        in_queue.clear();
        initialized = true;
        return SUCCESS;
    }
}

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

int Interpreter::stop_interpreter()
{
    if(running)
    {
        running = false;
        in_queue.clear();
        return SUCCESS;
    }
    else
    {
        return FAILURE;
    }
}

int Interpreter::add_incoming_data(QByteArray data)
{
    in_queue.append(data);

    //TODO
    return SUCCESS;
}

int Interpreter::encode_outgoing_message(QString message)
{
    //TODO
    return SUCCESS;
}
