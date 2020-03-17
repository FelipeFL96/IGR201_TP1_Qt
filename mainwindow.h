#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "zone_de_dessin.h"
#define MESSAGE(x) std::cout<<x<<std::endl

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /*Menus*/
    QMenu *fileMenu, *toolMenu;

    /*Actions de la barre de menus*/
    QAction *openFileAction, *saveFileAction, *quitFileAction;
    QAction *drawToolAction, *selectToolAction, *moveToolAction, *eraseToolAction;

public slots:
    void changeTool(QAction*);

protected:
    void closeEvent(QCloseEvent*);

private slots:
    void open(void);
    void save(void);

    void on_drawToolButton_clicked(void);
    void on_selectToolButton_clicked(void);
    void on_moveToolButton_clicked(void);
    void on_eraseToolButton_clicked(void);
    void on_formComboBox_currentIndexChanged(int);
    void on_colorComboBox_currentIndexChanged(int);
    void on_widthSpinBox_valueChanged(int);
    void on_solidStyleRadioButton_clicked(void);
    void on_dashedStyleRadioButton_clicked(void);
    void on_dottedStyleRadioButton_clicked(void);
    void on_dashDotStyleRadioButton_clicked(void);

    void onDessinSelected(void);
    void onDessinUnselected(void);


private:
    Ui::MainWindow *ui;
    zone_de_dessin *zd;

};

#endif // MAINWINDOW_H
