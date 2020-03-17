#include "dessin.h"
#include <QPainter>

/*
 * Constructeurs
 */
Dessin::Dessin()
{

}

Dessin::Dessin(Form form, QPoint start, QPoint end, QColor color, int width, Qt::PenStyle style) :
    dessinForm(form), dessinStartPoint(start), dessinEndPoint(end), dessinColor(color), dessinWidth(width), dessinStyle(style)
{

}

/*
 * Accesseurs
 */
Form Dessin::getDessinForm() { return dessinForm; }

QPoint Dessin::getStartPoint() { return dessinStartPoint; }

QPoint Dessin::getEndPoint() { return dessinEndPoint; }

QColor Dessin::getColor() { return dessinColor; }

int Dessin::getWidth() { return dessinWidth; }

Qt::PenStyle Dessin::getStyle() { return dessinStyle; }

/*
 * Mutateurs
 */
void Dessin::setDessinForm(Form form) { dessinForm = form; }

void Dessin::setStartPoint(QPoint startPoint) { dessinStartPoint = startPoint; }

void Dessin::setEndPoint(QPoint endPoint) { dessinEndPoint = endPoint; }

void Dessin::setColor(QColor color) { dessinColor = color; }

void Dessin::setWidth(int width) { dessinWidth = width; }

void Dessin::setStyle(Qt::PenStyle style) { dessinStyle = style; }

/*
 * Méthodes
 */

QPainterPath Dessin::generatePath() {
    QPainterPath dessinPath;
    if (dessinForm == Form::Line) {
        QPolygon line;
        line.setPoints(2, dessinStartPoint.x(), dessinStartPoint.y(), dessinEndPoint.x(), dessinEndPoint.y());
        dessinPath.addPolygon(line);
    }
    else if (dessinForm == Form::Rectangle) {
        dessinPath.addRect(dessinStartPoint.x(), dessinStartPoint.y(), dessinEndPoint.x() - dessinStartPoint.x(), dessinEndPoint.y() - dessinStartPoint.y());
    }
    else if (dessinForm == Form::Ellipse) {
        dessinPath.addEllipse(dessinStartPoint, dessinEndPoint.x() - dessinStartPoint.x(), dessinEndPoint.y() - dessinStartPoint.y());
    }
    return dessinPath;
}

QPen Dessin::generatePen() {
    QPen dessinPen;
    dessinPen.setColor(dessinColor);
    dessinPen.setWidth(dessinWidth);
    dessinPen.setStyle(dessinStyle);
    return dessinPen;
}

void Dessin::moveDessin(QPoint movement) {
    dessinStartPoint += movement;
    dessinEndPoint += movement;
}

bool operator==(const Dessin& a, const Dessin& b) {
    return &a == &b;
}
