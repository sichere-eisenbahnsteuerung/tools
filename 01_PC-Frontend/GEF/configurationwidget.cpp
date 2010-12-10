#include "configurationwidget.h"
#include "ui_configurationwidget.h"

ConfigurationWidget::ConfigurationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigurationWidget)
{
    ui->setupUi(this);
}

ConfigurationWidget::~ConfigurationWidget()
{
    delete ui;
}



/** @brief  Übergibt den Dateinamen der Config-Datei
  *
  *
  * @param  filename    Dateiname der *.ini Datei
  *
  * @return     SUCCESS     Einstellungen wurden erfolgreich übergeben.
  *             FAILURE     Einstellungen konnten nicht übergeben werden.
  *
  * @todo   Status message einführen
  */
int ConfigurationWidget::configure_config_widget(QString filename)
{
    this->filename = filename;
    settings = new QSettings(filename,QSettings::IniFormat,this);
    return SUCCESS;
}


/** @brief  Lädt die Einstellungen aus der .ini Datei in die GUI
  *
  * @return     SUCCESS     Einstellungen wurden erfolgreich geladen.
  *             FAILURE     Einstellungen konnten nicht geladen werden.
  *
  * @todo   Status Messages
  * @todo   Alle Einstellungen nachpflegen (auch in gui)
  */
int ConfigurationWidget::settings_load()
{
    settings->sync();
    settings->beginGroup("Allgemein");

    settings->endGroup();
    settings->beginGroup("Seriell");
    current_config_serial.comport = settings->value("comport", "1").toUInt();
    ui->serial_combo_comport->setCurrentIndex(settings->value("comport", "1").toInt()-1);
    current_config_serial.baudrate = settings->value("baudrate", "9600").toUInt();
    ui->serial_combo_baudrate->setCurrentIndex(ui->serial_combo_baudrate->findText(settings->value("baudrate", "9600").toString(),Qt::MatchExactly));
    current_config_serial.databits = settings->value("datenbits", "8").toUInt();
    ui->serial_combo_datenbits->setCurrentIndex(ui->serial_combo_datenbits->findText(settings->value("datenbits", "8").toString(),Qt::MatchExactly));
//    current_config_serial.parity = settings->value("paritaet", "Keine")
    ui->serial_combo_paritaet->setCurrentIndex(ui->serial_combo_paritaet->findText(settings->value("paritaet", "Keine").toString(),Qt::MatchExactly));
//    current_config_serial
    ui->serial_combo_stopbits->setCurrentIndex(ui->serial_combo_stopbits->findText(settings->value("stopbits", "1").toString(),Qt::MatchExactly));
//    current_config_serial
    ui->serial_combo_flusssteuerung->setCurrentIndex(ui->serial_combo_flusssteuerung->findText(settings->value("flusssteuerung", "Keine").toString(),Qt::MatchExactly));
//    current_config_serial
    ui->serial_line_RIT->setText(settings->value("RIT","0").toString());
//    current_config_serial
    ui->serial_line_RTTC->setText(settings->value("RTTC", "0").toString());
//    current_config_serial
    ui->serial_line_RTTM->setText(settings->value("RTTM", "0").toString());
//    current_config_serial
    ui->serial_line_WTTC->setText(settings->value("WTTC", "0").toString());
//    current_config_serial
    ui->serial_line_WTTM->setText(settings->value("WTTM", "0").toString());
    settings->endGroup();
    settings->beginGroup("Logging");
    ui->log_check_activate->setChecked(settings->value("activate", "true").toBool());
    ui->log_check_append->setChecked(settings->value("append", "true").toBool());
    ui->log_check_timestamp->setChecked(settings->value("timestamp", "true").toBool());
    ui->log_line_namebase->setText(settings->value("namebase").toString());
    settings->endGroup();

    return SUCCESS;
}


/** @brief  Speichert die aktuellen Einstellungen in der .ini Datei
  *
  * @return     SUCCESS     Einstellungen wurden erfolgreich gespeichert.
  *             FAILURE     Einstellungen konnten nicht gespeichert werden.
  *
  * @todo   status messages
  * @todo   einstellungen nachpflegen,siehe load settings
  */
int ConfigurationWidget::settings_save()
{
    /*
    settings->beginGroup("Allgemein");

    //TODO: Index vergleich prüfen (flussteuerung, paritaet,...)

    settings->endGroup();
    settings->beginGroup("Seriell");
    settings->setValue("comport", ui->serial_combo_comport->currentIndex()+1);
    settings->setValue("baudrate", ui->serial_combo_baudrate->currentText());
    settings->setValue("datenbits", ui->serial_combo_datenbits->currentText());
    settings->setValue("paritaet", ui->serial_combo_paritaet->currentIndex());
    settings->setValue("stopbits", ui->serial_combo_stopbits->currentText());
    settings->setValue("flusssteuerung", ui->serial_combo_flusssteuerung->currentIndex());
    settings->setValue("RIT",ui->serial_line_RIT->text());
    settings->setValue("RTTC",ui->serial_line_RTTC->text());
    settings->setValue("RTTM",ui->serial_line_RTTM->text());
    settings->setValue("WTTC",ui->serial_line_WTTC->text());
    settings->setValue("WTTM",ui->serial_line_WTTM->text());
    settings->endGroup();
    settings->beginGroup("Logging");
    settings->setValue("activate", ui->config_log_check_activate->isChecked());
    settings->setValue("append", ui->config_log_check_append->isChecked());
    settings->setValue("timestamp", ui->config_log_check_timestamp->isChecked());
    settings->setValue("namebase", ui->config_log_line_namebase->text());
    settings->endGroup();

    settings->sync();
*/
    return SUCCESS;
}


/////////////////////////GETTER///////////////////////////////////


/** @brief  Gibt die Konfiguration der GUI zurück.
  *
  * @return     GuiConfig   aktulle konfiguration
  *
  * @todo   Status message einführen
  * @todo   ggf daten vorher neu laden
  */
GuiConfig ConfigurationWidget::get_config_gui()
{
    return current_config_gui;
}



/** @brief  Gibt die Konfiguration der Konsole zurück.
  *
  * @return     ConsoleConfig   aktuelle konfiguration
  *
  * @todo   Status message einführen
  * @todo   ggf daten vorher neu laden
  */
ConsoleConfig ConfigurationWidget::get_config_console()
{
    return current_config_console;
}



/** @brief  Gibt die Konfiguration des Interpreters zurück.
  *
  * @return     InterpreterConfig   aktuelle konfiguration
  *
  * @todo   Status message einführen
  * @todo   ggf daten vorher neu laden
  */
InterpreterConfig ConfigurationWidget::get_config_interpreter()
{
    return current_config_interpreter;
}



/** @brief  Gibt die Konfiguration der Seriellen Schnittstelle zurück.
  *
  * @return     SerialConfig   aktuelle konfiguration
  *
  * @todo   Status message einführen
  * @todo   ggf daten vorher neu laden
  */
SerialConfig ConfigurationWidget::get_config_serial()
{
    return current_config_serial;
}



/** @brief  Gibt die Konfiguration des Logs zurück.
  *
  * @return     LogConfig   aktuelle konfiguration
  *
  * @todo   Status message einführen
  * @todo   ggf daten vorher neu laden
  */
LogConfig ConfigurationWidget::get_config_log()
{
    return current_config_log;
}



/** @brief  Gibt die Konfiguration der Gesamtapplikation(MainWindow) zurück.
  *
  * @return     AppConfig   aktulle konfiguration
  *
  * @todo   Status message einführen
  * @todo   ggf daten vorher neu laden
  */
AppConfig ConfigurationWidget::get_config_app()
{
    return current_config_app;
}

/////////////////////////SLOTS////////////////////////////////////

void ConfigurationWidget::accept_button_pressed()
{
    //saven, und loaden
}



void ConfigurationWidget::cancel_button_pressed()
{
    //nur loaden
}

/////////////////////////PROTECTED/////////////////////////////////
void ConfigurationWidget::paintEvent( QPaintEvent *event )
{
    setMinimumSize(parentWidget()->size());
    setMaximumSize(parentWidget()->size());
    event->accept();
}
