#ifndef DESSIN_H
#define DESSIN_H

#include <QObject>
#include <QWidget>
#include <QDataStream>

enum class Form {
    Line,
    Rectangle,
    Ellipse
};

class Dessin
{
public:
    /*
     * Constructeurs
     */
    Dessin();
    Dessin(Form, QPoint, QPoint, QColor, int, Qt::PenStyle);

    /*
     * Accesseurs
     */
    Form getDessinForm();
    QPoint getStartPoint();
    QPoint getEndPoint();
    QColor getColor();
    int getWidth();
    Qt::PenStyle getStyle();

    /*
     * Mutateurs
     */
    void setDessinForm(Form);
    void setStartPoint(QPoint);
    void setEndPoint(QPoint);
    void setColor(QColor);
    void setWidth(int);
    void setStyle(Qt::PenStyle);

    /*
     * Méthodes
     */
    QPainterPath generatePath(void);
    QPen generatePen(void);
    void moveDessin(QPoint);

    /*
     * Opérateurs
     */
    friend QDataStream& operator<<(QDataStream&, const Dessin&);
    friend bool operator==(const Dessin&, const Dessin&);

private:
    Form dessinForm;
    QPoint dessinStartPoint, dessinEndPoint;
    QColor dessinColor;
    int dessinWidth;
    Qt::PenStyle dessinStyle;
};

#endif // DESSIN_H
