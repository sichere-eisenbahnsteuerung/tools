#include "guiwidget.h"

GuiWidget::GuiWidget(QWidget *parent) :
    QWidget(parent)
{
    active_time = 2000;
    running = false;
    stopping = false;
    initialized = false;
    configured = false;
}



/** @brief  Zeichnet das Widget komplett neu.
  *
  * @todo   parameter entferen oder beschreiben
  * @todo   positionierung der komponenten prüfen
  */
void GuiWidget::paintEvent( QPaintEvent * )
{
    emit status_message("Starte Frame-Zeichnung", SOURCE_GUI, TYPE_STATUS_LVL_4);

    qreal parentheight = parentWidget()->size().height();
    qreal parentwidth = parentWidget()->size().width();

    this->setMinimumSize(parentwidth, parentheight);

    QPainter paint(this);
    draw_rails(&paint, 25,25,parentheight-50,parentwidth*0.75);
    draw_all_sensors(&paint, 25,25,parentheight-50,parentwidth*0.75);
    draw_all_switches(&paint, 25,25,parentheight-50,parentwidth*0.75);
    draw_all_couplers(&paint, 25,25,parentheight-50,parentwidth*0.75);

    draw_train_data(&paint,60+parentwidth*0.75,25,parentheight-50, parentwidth*0.25-60);

    emit status_message("Frame-Zeichnung beendet", SOURCE_GUI, TYPE_STATUS_LVL_4);
}



/** @brief Übergibt eine Konfigurationsdatei an das Widget
  *
  * @param config   Konfigurationsdatei
  *
  * @return     SUCCESS     Konfiguration erfolgreich.
  *             FAILURE     Konfiguration war nicht möglich.
  */
int GuiWidget::configure_gui(GuiConfig config)
{
    if(!running && !stopping)
    {
        configuration = config;
        initialized = false;
        emit status_message("Konfiguration Erfolgreich", SOURCE_GUI, TYPE_STATUS_LVL_1);
        return SUCCESS;
    }
    else
    {
        emit status_message("Konfiguration fehlgeschlagen(Modul läuft noch)", SOURCE_GUI, TYPE_ERROR);
        return FAILURE;
    }

}



/** @brief  Übergibt die darzustellenden Daten an das Widget
  *
  * @param  events  darzustellende Daten
  *
  * @return     SUCCESS     Übergabe erfolgreich.
  *             FAILURE     Übergabe war nicht möglich.
  *
  * @todo   Übergabe und Speicherung von Zugdaten einführen
  * @todo   Fehler bei unsinnigen Daten ausgeben.
  */
int GuiWidget::display_data(QList<RailEvent> events)
{
    RailEvent current_event;
    for(int i = 0; i < events.length(); i++)
    {
        current_event = events.takeFirst();
        switch(current_event.object_type)
        {
        case RAIL_SENSOR:
            if(current_event.event == RAIL_EVENT_ACTIVATED)
            {
                active_sensors[current_event.object_number-1] = QTime::currentTime();
            }
            break;
        case RAIL_COUPLER:
            if(current_event.event == RAIL_EVENT_ACTIVATED)
            {
                active_couplers[current_event.object_number-1] = QTime::currentTime();
            }
            break;
        case RAIL_SWITCH:
            if(current_event.event == RAIL_EVENT_ACTIVATED)
            {
                active_switches[current_event.object_number-1] = QTime::currentTime();
                switch_positions[current_event.object_number-1] = current_event.data;
            }
            break;
        case RAIL_TRAIN:
            if(current_event.event == RAIL_EVENT_SPEED)
            {
                train_speed[current_event.object_number-1] = current_event.data;
            }
            break;
        default:

            break;
        }
    }

    emit status_message("Daten erhalten", SOURCE_GUI, TYPE_STATUS_LVL_4);
    return SUCCESS;
}



/** @brief  Zeichnet das Schienennetz
  *
  *         dx/dy beschreiben den künstlichen Ursprung auf der Leinwand.
  *         Height/width beschreiben die Größe
  *
  * @param  *paint  Pointer auf die verwendete Leinwand.
  * @param  dx      Abstand vom linken Rand der Leinwand
  * @param  dy      Abstand vom oberen Rand der Leinwand
  * @param  height  Höhe der Zeichnung
  * @param  width   Breite der Zeichnung
  *
  * @return     SUCCESS     Zeichnung erfolgreich.
  *             FAILURE     Zeichnung war nicht möglich.
  *
  * @todo   ggf. aufhübschen (echte schienenoptik), absolut sekundär
  */
int GuiWidget::draw_rails(QPainter *paint, int dx, int dy, int height, int width)
{
    paint->setBrush(QBrush(QColor("blue")));
    paint->setPen(QPen(QBrush(QColor("blue")), 10));

    paint->drawLine(dx,         dy+100,     dx,                 dy+height-50);      //links
    paint->drawLine(dx+50,      dy+50,      dx+width-50,        dy+50);             //oben
    paint->drawLine(dx+width,   dy+100,     dx+width,           dy+height-50);      //rechts
    paint->drawLine(dx+50,      dy+height,  dx+width-50,        dy+height);         //unten

    //paint->setPen(QPen(QBrush(QColor("red")), 10));
    paint->drawArc(QRect(dx,dy+50,100,100),90*16,90*16);    //oben links
    paint->drawArc(QRect(dx+width-100,dy+50,100,100),0*16,90*16); //oben rechts
    paint->drawArc(QRect(dx,dy+height-100,100,100),180*16,90*16);    //unten links
    paint->drawArc(QRect(dx+width-100,dy+height-100,100,100),270*16,90*16);    //unten rechts

    //innengleis
    paint->drawLine(dx+width*0.3, dy+(height*0.75), dx+width-50, dy+(height*0.75));
    paint->drawArc(QRect(dx+width-100,dy+(height*0.75)-100,100,100),270*16,90*16);

    //parallelgleis
    paint->drawLine(dx+100,      dy,      dx+width-100,        dy);
    paint->drawArc(QRect(dx+75, dy,50,50),90*16,90*16);
    paint->drawArc(QRect(dx+25,dy,50,50),270*16,90*16);
    paint->drawArc(QRect(dx+width-125,dy,50,50),0*16,90*16);
    paint->drawArc(QRect(dx+width-75,dy,50,50),180*16,90*16);

    return SUCCESS;
}



/** @brief  Zeichnet einen Sensor
  *
  * @param  *paint  Pointer auf die verwendete Leinwand.
  * @param  x       Abstand vom linken Rand der Leinwand
  * @param  y       Abstand vom oberen Rand der Leinwand
  * @param  active  wenn 'true' wird ein aktiver sensor gezeichnet.
  *
  * @return     SUCCESS     Zeichnung erfolgreich.
  *             FAILURE     Zeichnung war nicht möglich.
  *
  * @todo   ggf. aufhübschen, absolut sekundär
  */
int GuiWidget::draw_sensor(QPainter *paint, int x, int y, bool active)
{
    if(active)
    {
        paint->setBrush(QBrush(QColor("lime")));
        paint->setPen(QPen(QBrush(QColor("lime")), 10));
    }
    else
    {
        paint->setBrush(QBrush(QColor("red")));
        paint->setPen(QPen(QBrush(QColor("red")), 10));
    }

    paint->drawEllipse(x,y,15,15);

    return SUCCESS;
}




/** @brief  Zeichnet alle Sensoren neu.
  *
  *         dx/dy beschreiben den künstlichen Ursprung auf der Leinwand.
  *         Height/width beschreiben die Größe
  *
  * @param  *paint  Pointer auf die verwendete Leinwand.
  * @param  dx      Abstand vom linken Rand der Leinwand
  * @param  dy      Abstand vom oberen Rand der Leinwand
  * @param  height  Höhe der Zeichnung
  * @param  width   Breite der Zeichnung
  *
  * @return     SUCCESS     Zeichnung erfolgreich.
  *             FAILURE     Zeichnung war nicht möglich.
  *
  * @todo   fehler der einzelfunktionen sammeln und ggf mit FAILURE melden
  * @todo   sensor positionen anpassen
  */
int GuiWidget::draw_all_sensors(QPainter *paint, int dx, int dy, int height, int width)
{
    bool active[14];
    for(int i = 0; i < 14; i++)
    {
        if(active_sensors[i].msecsTo(QTime::currentTime()) <= active_time)
        {
            active[i] = true;
        }
        else
        {
            active[i] = false;
        }
    }

    draw_sensor(paint, dx+width-45,         dy+45,                  active[0]);
    draw_sensor(paint, dx+width-100,        dy+43,                  active[1]);
    draw_sensor(paint, dx+width*0.35,       dy+43,                  active[2]);
    draw_sensor(paint, dx+85,               dy+43,                  active[3]);
    draw_sensor(paint, dx+35,               dy+45,                  active[4]);
    draw_sensor(paint, dx-7,                dy+height-60,           active[5]);
    draw_sensor(paint, dx+width*0.5,        dy+height-7,            active[6]);
    draw_sensor(paint, dx+width-7,          dy+height*0.75,         active[7]);
    draw_sensor(paint, dx+width-7,          dy+height*0.5,          active[8]);
    draw_sensor(paint, dx+width-100,        dy-7,                   active[9]);
    draw_sensor(paint, dx+85,               dy-7,                   active[10]);
    draw_sensor(paint, dx+width-50,         dy+(height*0.75)-7,     active[11]);
    draw_sensor(paint, dx+width*0.6,        dy+(height*0.75)-7,     active[12]);
    draw_sensor(paint, dx+(width*0.3)+10,   dy+(height*0.75)-7,     active[13]);

    return SUCCESS;
}



/** @brief  Zeichnet eine Weiche
  *
  *         Mögliche Stellungen:
  *         0 = |
  *         1 = /
  *         2 = --
  *         3 = \
  *
  * @param  *paint      Pointer auf die verwendete Leinwand.
  * @param  x           Abstand vom linken Rand der Leinwand
  * @param  y           Abstand vom oberen Rand der Leinwand
  * @param  alignment   Gibt die Stellung der Weiche an
  * @param  active      wenn 'true' wird eine aktive Weiche gezeichnet.
  *
  * @return     SUCCESS     Zeichnung erfolgreich.
  *             FAILURE     Zeichnung war nicht möglich.
  *
  * @todo   ggf. aufhübschen, absolut sekundär
  */
int GuiWidget::draw_switch(QPainter *paint, int x, int y, int alignment, bool active)
{
    int size = 25;

    if(active)
    {
        paint->setBrush(QBrush(QColor("white")));
        paint->setPen(QPen(QBrush(QColor("lime")), 5));
    }
    else
    {
        paint->setBrush(QBrush(QColor("white")));
        paint->setPen(QPen(QBrush(QColor("black")), 5));
    }
    paint->drawRect(x,y,size,size);

    paint->setBrush(QBrush(QColor("black")));
    paint->setPen(QPen(QBrush(QColor("black")), 5));

    QPoint p1, p2;
    switch(alignment)
    {
    case 0:
        p1 = QPoint(x + size*0.5, y + 5);
        p2 = QPoint(x + size*0.5, y+size-5);
        break;
    case 1:
        p1 = QPoint(x + size - 5, y + 5);
        p2 = QPoint(x + 5, y + size - 5);
        break;
    case 2:
        p1 = QPoint(x + size - 5, y + size*0.5);
        p2 = QPoint(x + 5, y + size*0.5);
        break;
    case 3:
        p1 = QPoint(x + size - 5, y + size - 5);
        p2 = QPoint(x + 5, y + 5);
        break;
    default:
        p1 = QPoint(x + size*0.5 - 1, y + size*0.5);
        p2 = QPoint(x + size*0.5 + 1, y + size*0.5);
        break;
    }

    paint->drawLine(p1,p2);

    return SUCCESS;
}



/** @brief  Zeichnet alle Weichen neu.
  *
  *         dx/dy beschreiben den künstlichen Ursprung auf der Leinwand.
  *         Height/width beschreiben die Größe
  *
  * @param  *paint  Pointer auf die verwendete Leinwand.
  * @param  dx      Abstand vom linken Rand der Leinwand
  * @param  dy      Abstand vom oberen Rand der Leinwand
  * @param  height  Höhe der Zeichnung
  * @param  width   Breite der Zeichnung
  *
  * @return     SUCCESS     Zeichnung erfolgreich.
  *             FAILURE     Zeichnung war nicht möglich.
  *
  * @todo   fehler der einzelfunktionen sammeln und ggf mit FAILURE melden
  * @todo   weichen positionen anpassen
  */
int GuiWidget::draw_all_switches(QPainter *paint, int dx, int dy, int height, int width)
{
    bool active[3];
    for(int i = 0; i < 3; i++)
    {
        if(active_switches[i].msecsTo(QTime::currentTime()) <= active_time)
        {
            active[i] = true;
        }
        else
        {
            active[i] = false;
        }
    }


    draw_switch(paint, dx+width-77,     dy+35,              switch_positions[0],    active[0]);       //stellungen 2 und 3
    draw_switch(paint, dx+57,           dy+35,              switch_positions[1],    active[1]);       //stellungen 1 und 2
    draw_switch(paint, dx+width-13,     dy+height*0.75-40,  switch_positions[2],    active[2]);       //stellungen 0 und 1

    return SUCCESS;
}




/** @brief  Zeichnet einen Entkoppler
  *
  * @param  *paint  Pointer auf die verwendete Leinwand.
  * @param  x       Abstand vom linken Rand der Leinwand
  * @param  y       Abstand vom oberen Rand der Leinwand
  * @param  active  wenn 'true' wird ein aktiver Entkoppler gezeichnet.
  *
  * @return     SUCCESS     Zeichnung erfolgreich.
  *             FAILURE     Zeichnung war nicht möglich.
  *
  * @todo   ggf. aufhübschen, absolut sekundär
  */
int GuiWidget::draw_coupler(QPainter *paint, int x, int y, bool active)
{
    int size = 30;

    if(active)
    {
        paint->setBrush(QBrush(QColor("lime")));
        paint->setPen(QPen(QBrush(QColor("black")), 5));
    }
    else
    {
        paint->setBrush(QBrush(QColor("red")));
        paint->setPen(QPen(QBrush(QColor("black")), 5));
    }

    QPoint p1,p2,p3;
    p1 = QPoint(x,y+int(0.5*size*sqrt(3)));
    p2 = QPoint(x+size*0.5, y);
    p3 = QPoint(x+size,y+int(0.5*size*sqrt(3)));
    QVector<QPoint> point_vector;
    point_vector.append(p1);
    point_vector.append(p2);
    point_vector.append(p3);

    QPolygon poly(point_vector);

    paint->drawPolygon(poly);

    return SUCCESS;
}



/** @brief  Zeichnet alle Entkoppler neu.
  *
  *         dx/dy beschreiben den künstlichen Ursprung auf der Leinwand.
  *         Height/width beschreiben die Größe
  *
  * @param  *paint  Pointer auf die verwendete Leinwand.
  * @param  dx      Abstand vom linken Rand der Leinwand
  * @param  dy      Abstand vom oberen Rand der Leinwand
  * @param  height  Höhe der Zeichnung
  * @param  width   Breite der Zeichnung
  *
  * @return     SUCCESS     Zeichnung erfolgreich.
  *             FAILURE     Zeichnung war nicht möglich.
  *
  * @todo   fehler der einzelfunktionen sammeln und ggf mit FAILURE melden
  * @todo   Entkoppler positionen anpassen
  */
int GuiWidget::draw_all_couplers(QPainter *paint, int dx, int dy, int height, int width)
{
    bool active[2];
    for(int i = 0; i < 2; i++)
    {
        if(active_couplers[i].msecsTo(QTime::currentTime()) <= active_time)
        {
            active[i] = true;
        }
        else
        {
            active[i] = false;
        }
    }

    draw_coupler(paint,     dx+width*0.4,   dy+37,                  active[0]);
    draw_coupler(paint,     dx+width*0.5,   dy+(height*0.75)-13,    active[1]);

    return SUCCESS;
}


/** @brief  Zeichnet alle Zugdaten neu.
  *
  *         dx/dy beschreiben den künstlichen Ursprung auf der Leinwand.
  *         Height/width beschreiben die Größe
  *
  * @param  *paint  Pointer auf die verwendete Leinwand.
  * @param  dx      Abstand vom linken Rand der Leinwand
  * @param  dy      Abstand vom oberen Rand der Leinwand
  * @param  height  Höhe der Zeichnung
  * @param  width   Breite der Zeichnung
  *
  * @return     SUCCESS     Zeichnung erfolgreich.
  *             FAILURE     Zeichnung war nicht möglich.
  *
  * @todo   Zugdaten positionen anpassen
  * @todo   datenstruktur für zugdaten anlegen und darstellen.
  * @todo   änderungen ggf farbig
  */
int GuiWidget::draw_train_data(QPainter *paint, int dx, int dy, int height, int width)
{
    QString traintext("AKTUELLE ZUGDATEN\r\n");
    paint->setBrush(QBrush(QColor("black")));
    paint->setPen(QPen(QBrush(QColor("black")), 10));

    traintext.append("Zug 1\r\n");
    traintext.append(tr("Geschwindigkeit: %1\r\n").arg(train_speed[0]));
    traintext.append("\r\n");
    traintext.append("Zug 2\r\n");
    traintext.append(tr("Geschwindigkeit: %1XXXX\r\n").arg(train_speed[1]));

    paint->drawText(dx, dy, width, height,Qt::AlignLeft, traintext);

    return SUCCESS;
}



/** @brief  Initialisiert das Widget mit der aktuellen Konfiguration.
  *
  * @return     SUCCESS     Initialisierung erfolgreich.
  *             FAILURE     Initialisierung war nicht möglich.
  */
int GuiWidget::initialize_gui()
{
    if(!running && !stopping && configured)
    {
        emit status_message("Starte Initialisierung", SOURCE_GUI, TYPE_STATUS_LVL_2);
        last_paint = QTime::currentTime();

        for(int i = 0; i < 14; i++)
        {
            active_sensors[i] = QTime::currentTime();
        }
        for(int i = 0; i < 3; i++)
        {
            active_switches[i] = QTime::currentTime();
        }
        for(int i = 0; i < 2; i++)
        {
            active_couplers[i] = QTime::currentTime();
        }

        for(int i = 0; i < 3; i++)
        {
            switch_positions[i] = 100;
        }

        for(int i = 0; i < 2; i++)
        {
            train_speed[i] = 0;
        }

        emit status_message("Modul initialisiert", SOURCE_GUI, TYPE_STATUS_LVL_1);

        return SUCCESS;
    }
    else
    {
        emit status_message("Modul initialisiert", SOURCE_GUI, TYPE_ERROR);
        return FAILURE;
    }

}



/** @brief  Startet das Widget.
  *
  * @return     SUCCESS     Initialisierung erfolgreich.
  *             FAILURE     Initialisierung war nicht möglich.
  *
  */
int GuiWidget::start_gui()
{
    if(!running && !stopping && initialized)
    {
        emit status_message("Starte Modul", SOURCE_GUI, TYPE_STATUS_LVL_2);
        running = true;
        stopping = false;
        run_gui();
        emit status_message("Modul gestartet", SOURCE_GUI, TYPE_STATUS_LVL_1);
        return SUCCESS;
    }
    else
    {
        emit status_message("Kann Modul nicht starten", SOURCE_GUI, TYPE_ERROR);
        return FAILURE;
    }

}



/** @brief  Setzt das Stop-Flag der GUI. Stop erfolgt je nach FPS
  *
  * @return     SUCCESS     Initialisierung erfolgreich.
  *             FAILURE     Initialisierung war nicht möglich.
  */
int GuiWidget::stop_gui()
{
    if(running && !stopping)
    {
        emit status_message("Stoppe Modul", SOURCE_GUI, TYPE_STATUS_LVL_2);
        stopping = true;
        return SUCCESS;
    }
    else
    {
        emit status_message("Kann Modul nicht stoppen", SOURCE_GUI, TYPE_ERROR);
        return FAILURE;
    }
}



/** @brief  Hauptschleife des Widgets.
  *
  *         Ruft sich selbst auf.
  */
void GuiWidget::run_gui()
{
    repaint();
    if(running && !stopping)
    {
        int timer = 1000 / configuration.frames_per_second;
        QTimer::singleShot(timer, this, SLOT(run_gui()));
    }

    running = false;
    stopping = false;
    emit status_message("Modul gestoppt", SOURCE_GUI, TYPE_STATUS_LVL_1);
}



//////////////////////////////////////DEBUG//////////////////////////////////////////////


#ifdef DEBUG_MODE

void GuiWidget::debug_command(QStringList commands)
{
    QString section = commands.takeFirst();

    if(section == "sensor")
    {
        while(!commands.isEmpty())
        {
            QString id_string = commands.takeFirst();
            int id = id_string.toInt()-1;
            if(id < 14)active_sensors[id] = QTime::currentTime();
        }
    }
    else if(section == "coupler")
    {
        while(!commands.isEmpty())
        {
            QString id_string = commands.takeFirst();
            int id = id_string.toInt()-1;
            if(id < 2)active_couplers[id] = QTime::currentTime();
        }
    }
    else if(section == "switch")
    {
        if(commands.length() % 2 == 0)
        {
            while(!commands.isEmpty())
            {
                QString id_string = commands.takeFirst();
                QString posi_string = commands.takeFirst();
                int id = id_string.toInt()-1;
                int posi = posi_string.toInt();

                if(id < 3 && posi < 4)
                {
                    switch_positions[id] = posi;
                    active_switches[id] = QTime::currentTime();
                }
            }
        }
    }
}

#endif
