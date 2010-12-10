#ifndef FILELOGGING_H
#define FILELOGGING_H

#include <QObject>
#include "defines.h"
#include "includes.h"

class FileLogging : public QObject
{
    Q_OBJECT
public:
    explicit FileLogging(QObject *parent = 0);
    int log_message(QString message, int source, int type);
    int start_log();
    int stop_log();
    int initialize_log();
    int configure_log(LogConfig config);
    bool is_logging();

public slots:

private:
    LogConfig configuration;
    QFile *logfile;
    bool running;
    bool initialized;


signals:
    void status_message(QString message, int source, int type);
};

#endif // FILELOGGING_H
