#include <iostream>
#include <iterator>
#include <QPainter>
#include "mainwindow.h"
#include "zone_de_dessin.h"
#include "dessin.h"

/*
 * Constructeurs
 */
zone_de_dessin::zone_de_dessin(QWidget *parent) : QWidget(parent),
    tool(Tool::Draw),
    dessinForm(Form::Line),
    dessinColor(Qt::GlobalColor::black), dessinWidth(1), dessinStyle(Qt::PenStyle::SolidLine),
    dessinStart(0,0), dessinEnd(0,0)
{
    MESSAGE("Zone de Dessin crée");
}

/*
 * Peinture
 */

void zone_de_dessin::paintEvent(QPaintEvent *e) {
    QWidget::paintEvent(e);
    QPen pen;
    QPainter painter(this);
    QPainterPath newPath;

    //Afichage de la nouvelle forme
    Dessin newDessin(dessinForm, dessinStart, dessinMiddle, dessinColor, dessinWidth, dessinStyle);

    if (tool == Tool::Draw  && dessinEnd != dessinStart) {
        newPath = newDessin.generatePath();
        pen = newDessin.generatePen();
        painter.setPen(pen);
        painter.drawPath(newPath);
    }
    else if(tool == Tool::Select) {
        pen.setColor(Qt::cyan);
        pen.setWidth(1);
        pen.setStyle(Qt::DashLine);

        selector.setCoords(newDessin.getStartPoint().x(), newDessin.getStartPoint().y(), newDessin.getEndPoint().x(), newDessin.getEndPoint().y());
        newPath.addRect(selector);
        if (!releaseEvent) {
            painter.setPen(pen);
            painter.drawPath(newPath);
        }
    }

    // Affichage des formes déjà dessinées
    QList<Dessin>::iterator d;
    for(d = drawnDessins.begin(); d != drawnDessins.end(); ++d) {
        QPainterPath path;

        path = d->generatePath();
        pen = d->generatePen();

        if (tool == Tool::Select && path.intersects(selector)) {
            pen.setColor(Qt::cyan);
        }
        if (!selectedDessins.empty() && (tool == Tool::Select || tool == Tool::Move)) {
            QList<Dessin*>::iterator s;
            for (s = selectedDessins.begin(); s != selectedDessins.end(); ++s) {
                if (&(*d) == &(*(*s)))
                    pen.setColor(Qt::cyan);
            }
        }

        painter.setPen(pen);
        painter.drawPath(path);
    }
}

/*
 * Événements
 */

void zone_de_dessin::mousePressEvent(QMouseEvent *e) {
    releaseEvent = false;

    if (e->button() == Qt::LeftButton) {
        dessinStart = e->pos();
    }
}

void zone_de_dessin::mouseMoveEvent(QMouseEvent *e) {
    releaseEvent = false;

    if (tool == Tool::Move && !selectedDessins.empty()) {
        moveSelectedDessins(e->pos() - dessinMiddle);
    }
    dessinMiddle = e->pos();

    update();
}

void zone_de_dessin::mouseReleaseEvent(QMouseEvent *e) {
    changed = true;
    releaseEvent = true;

    if(e->button() == Qt::LeftButton) {
        dessinEnd = e->pos();

        if (tool == Tool::Draw && dessinEnd != dessinStart) {
            Dessin d(dessinForm, dessinStart, dessinEnd, dessinColor, dessinWidth, dessinStyle);
            drawnDessins.append(d);
        }
        else if (tool == Tool::Select) {
            if (dessinEnd == dessinStart) {
                selectedDessins.clear();
                emit dessinUnselected();
                MESSAGE("Liste de sélection vidée");
            }
            else {
                updateSelectedList();
            }
        }
        update();
    }
}

/*
 * Méthodes
 */

Tool zone_de_dessin::currentTool() {
    return tool;
}

void zone_de_dessin::changeTool(Tool chosenTool) {
    tool = chosenTool;
    if (chosenTool == Tool::Draw) {
        clearSelection();
    }
}

void zone_de_dessin::changeForm(Form form) {
    dessinForm = form;
}

void zone_de_dessin::changeColor(QColor color) {
    if (tool == Tool::Select)
        selectedChangeColor(color);
    else
        dessinColor = color;
}

void zone_de_dessin::changeWidth(int width) {
    if (tool == Tool::Select)
        selectedChangeWidth(width);
    else
        dessinWidth = width;
}

void zone_de_dessin::changeStyle(Qt::PenStyle style) {
    if (tool == Tool::Select)
        selectedChangeStyle(style);
    else
        dessinStyle = style;
}

void zone_de_dessin::saved() {
    changed = false;
}

bool zone_de_dessin::hasChanged() {
    return changed;
}

void zone_de_dessin::eraseSelectedDessins() {
    QList<Dessin*>::iterator s;

    for (s = selectedDessins.begin(); s != selectedDessins.end(); ++s) {
        int i = drawnDessins.indexOf(*(*s));
        drawnDessins.removeAt(i);
    }
    update();
}

void zone_de_dessin::updateSelectedList() {
    QList<Dessin>::iterator d;

    selectedDessins.clear();
    int count = 0;
    for (d = drawnDessins.begin(); d != drawnDessins.end(); ++d, count++) {
        if (d->generatePath().intersects(selector)) {
            selectedDessins << &(*d);
        }
    }
    if (selectedDessins.size() > 0) {
        emit dessinSelected();
    }
    MESSAGE("Dessins sélectionés: " << selectedDessins.size());
}

void zone_de_dessin::clearSelection() {
    selectedDessins.clear();

    dessinStart.setX(0);
    dessinStart.setY(0);
    dessinMiddle.setX(0);
    dessinMiddle.setY(0);
    dessinEnd.setX(0);
    dessinEnd.setY(0);

    emit dessinUnselected();
    update();
}

void zone_de_dessin::selectedChangeColor(QColor color) {
    QList<Dessin*>::iterator s;

    for(s = selectedDessins.begin(); s != selectedDessins.end(); ++s) {
        (*s)->setColor(color);
    }
    MESSAGE("Couleur changée pour " << selectedDessins.size() << " dessins");

    clearSelection();
    update();
}

void zone_de_dessin::selectedChangeWidth(int width) {
    QList<Dessin*>::iterator s;

    for(s = selectedDessins.begin(); s != selectedDessins.end(); ++s) {
        (*s)->setWidth(width);
    }
    MESSAGE("Épaisseur changée pour " << selectedDessins.size() << " dessins");

    update();
}

void zone_de_dessin::selectedChangeStyle(Qt::PenStyle style) {
    QList<Dessin*>::iterator s;

    for(s = selectedDessins.begin(); s != selectedDessins.end(); ++s) {
        (*s)->setStyle(style);
    }
    MESSAGE("Style de trait changée pour " << selectedDessins.size() << " dessins");

    clearSelection();
    update();
}

void zone_de_dessin::moveSelectedDessins(QPoint movement) {
    QList<Dessin*>::iterator s;

    for (s = selectedDessins.begin(); s != selectedDessins.end(); ++s) {
        (*s)->moveDessin(movement);
    }
}
