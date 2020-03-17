#ifndef ZONE_DE_DESSIN_H
#define ZONE_DE_DESSIN_H

#include <QWidget>
#include <QMouseEvent>
#include "dessin.h"

enum class Tool{
    None,
    Draw,
    Select,
    Move
};

class zone_de_dessin : public QWidget
{
    Q_OBJECT
public:
    zone_de_dessin(QWidget *parent = nullptr);

    Tool currentTool(void);
    void changeTool(Tool);
    void changeForm(Form);
    void changeColor(QColor);
    void changeWidth(int);
    void changeStyle(Qt::PenStyle);
    void saved();
    bool hasChanged();
    void eraseSelectedDessins();

    QList<Dessin> drawnDessins;

protected:
    virtual void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);

private:
    void updateSelectedList(void);
    void clearSelection(void);
    void selectedChangeColor(QColor);
    void selectedChangeStyle(Qt::PenStyle);
    void selectedChangeWidth(int);
    void moveSelectedDessins(QPoint);

    Tool tool;
    QCursor cursor;
    Form dessinForm;
    QColor dessinColor;
    int dessinWidth;
    Qt::PenStyle dessinStyle;
    QPoint dessinStart, dessinMiddle, dessinEnd;
    QRect selector;
    QList<Dessin*> selectedDessins;
    bool releaseEvent = false;
    bool changed = false;

signals:
    void dessinSelected(void);
    void dessinUnselected(void);

public slots:


};

#endif // ZONE_DE_DESSIN_H
