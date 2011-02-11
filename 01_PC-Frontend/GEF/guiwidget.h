#ifndef GUIWINDOW_H
#define GUIWINDOW_H

#include "includes.h"
#include "defines.h"


/** @brief  Stellt die eingehenden Daten grafisch dar.
  *
  *         Die Klasse stellt alle Informationen über das Schienennetz grafisch dar.
  *         So lässt sich leicht erkennen wie die Situation auf dem Schienennetz sein sollte.
  */
class GuiWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GuiWidget(QWidget *parent = 0);
    int configure_gui(GuiConfig config);
    int initialize_gui();
    int start_gui();
    int stop_gui();
    bool is_terminated();

    int display_data(QList<RailEvent> events);

#ifdef DEBUG_MODE
    void debug_command(QStringList commands);
#endif

private:
    bool running, stopping, initialized, configured;
    QTime active_sensors[14];  //Enthält den Zeitpunkt der letzten aktivierung
    QTime active_switches[3];
    QTime active_couplers[2];

    QTime last_paint;

    GuiConfig configuration;

    int switch_positions[3];
    int train_speed[2];

    int draw_rails(QPainter *paint, int dx, int dy, int height, int width);

    int draw_sensor(QPainter *paint, int x, int y, bool active);
    int draw_all_sensors(QPainter *paint, int dx, int dy, int height, int width);

    int draw_switch(QPainter *paint, int x, int y, int alignment, bool active);
    int draw_all_switches(QPainter *paint, int dx, int dy, int height, int width);

    int draw_coupler(QPainter *paint, int x, int y, bool active);
    int draw_all_couplers(QPainter *paint, int dx, int dy, int height, int width);

    int draw_train_data(QPainter *paint, int dx, int dy, int height, int width);

private slots:
    void run_gui();

protected:
    void paintEvent( QPaintEvent * );


signals:
    void status_message(QString message, int source, int type);

public slots:

};

#endif // GUIWINDOW_H
