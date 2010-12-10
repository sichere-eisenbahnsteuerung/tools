#ifndef CONFIGURATIONWIDGET_H
#define CONFIGURATIONWIDGET_H

#include <QWidget>
#include "defines.h"
#include "includes.h"

namespace Ui {
    class ConfigurationWidget;
}

class ConfigurationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigurationWidget(QWidget *parent = 0);
    ~ConfigurationWidget();

    int configure_config_widget(QString filename);

    GuiConfig get_config_gui();
    ConsoleConfig get_config_console();
    InterpreterConfig get_config_interpreter();
    SerialConfig get_config_serial();
    LogConfig get_config_log();
    AppConfig get_config_app();

private:
    Ui::ConfigurationWidget *ui;
    QString filename;
    QSettings *settings;

    GuiConfig           current_config_gui;
    ConsoleConfig       current_config_console;
    InterpreterConfig   current_config_interpreter;
    SerialConfig        current_config_serial;
    LogConfig           current_config_log;
    AppConfig           current_config_app;

    int settings_save();
    int settings_load();

private slots:
    void accept_button_pressed();
    void cancel_button_pressed();

protected:
    void paintEvent( QPaintEvent *event );
};

#endif // CONFIGURATIONWIDGET_H
