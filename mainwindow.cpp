#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRadioButton>
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->setMaximumSize(1000, 700);

    ui->setupUi(this);
    zd = ui->zoneDessin;
    zd->setCursor(Qt::CrossCursor);
    zd->changeTool(Tool::Draw);
    ui->moveToolButton->setEnabled(false);
    ui->eraseToolButton->setEnabled(false);

    QMenuBar *drawMenuBar = this->menuBar();
    fileMenu = drawMenuBar->addMenu(tr("&File"));
    toolMenu = drawMenuBar->addMenu(tr("&Tools"));

    QActionGroup *fileActionGroup = new QActionGroup(this);
    openFileAction = fileActionGroup->addAction(tr("&Open"));
    saveFileAction = fileActionGroup->addAction(tr("&Save"));
    quitFileAction = fileActionGroup->addAction(tr("&Quit"));
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(saveFileAction);
    fileMenu->addAction(quitFileAction);
    connect(openFileAction, SIGNAL(triggered()), this, SLOT(open()));
    connect(saveFileAction, SIGNAL(triggered()), this, SLOT(save()));
    connect(quitFileAction, SIGNAL(triggered()), this, SLOT(close()));

    QActionGroup *toolActionGroup = new QActionGroup(this);
    drawToolAction = toolActionGroup->addAction("&Draw");
    selectToolAction = toolActionGroup->addAction("&Select");
    moveToolAction = toolActionGroup->addAction("&Move");
    eraseToolAction = toolActionGroup->addAction("&Erase");
    toolMenu->addAction(drawToolAction);
    toolMenu->addAction(selectToolAction);
    toolMenu->addAction(moveToolAction);
    toolMenu->addAction(eraseToolAction);
    connect(toolActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(changeTool(QAction*)));

    connect(zd, SIGNAL(dessinSelected()), this, SLOT(onDessinSelected()));
    connect(zd, SIGNAL(dessinUnselected()), this, SLOT(onDessinUnselected()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::open() {
    QString flux;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("Images (*.frm);;All Files (*)"));
    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Impossible d'sauvegarder dans le fichier donné"), file.errorString());
            return;
        }
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_4_5);

        zd->drawnDessins.clear();
        int size;
        in >> size;
        for (int i = 0; i != size; i++) {
            int form;
            QPoint startPoint, endPoint;
            QColor color;
            int width;
            int style;

            in >> form >> startPoint >> endPoint >> color >> width >> style;
            Dessin d(static_cast<Form>(form), startPoint, endPoint, color, width, static_cast<Qt::PenStyle>(style));
            zd->drawnDessins.append(d);
        }
        zd->update();
    }
}

void MainWindow::save() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "/home/sans_titre.frm", tr("Images (*.frm);;All Files (*)"));
    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Impossible d'ouvrir le fichier"), file.errorString());
            return;
        }
        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_4_5);

        int size = zd->drawnDessins.size();
        out << size;
        QList<Dessin>::iterator d;
        for (d = zd->drawnDessins.begin(); d != zd->drawnDessins.end(); ++d) {
            out << static_cast<int>(d->getDessinForm()) << d->getStartPoint() << d->getEndPoint() << d->getColor() << d->getWidth() << static_cast<int>(d->getStyle());
        }
        zd->saved();
    }
}

void MainWindow::on_drawToolButton_clicked()  {
    changeTool(drawToolAction);
    zd->changeTool(Tool::Draw);
}

void MainWindow::on_selectToolButton_clicked() {
    changeTool(selectToolAction);
    zd->changeTool(Tool::Select);
}

void MainWindow::on_moveToolButton_clicked() {
    changeTool(moveToolAction);
    zd->changeTool(Tool::Move);
}

void MainWindow::on_eraseToolButton_clicked() {
    zd->eraseSelectedDessins();
    changeTool(selectToolAction);
}

void MainWindow::on_formComboBox_currentIndexChanged(int index) {
    zd->changeForm(static_cast<Form>(index));
}

void MainWindow::on_colorComboBox_currentIndexChanged(int index) {
    switch (index) {
    case 0:
        zd->changeColor(Qt::black);
        break;
    case 1:
        zd->changeColor(Qt::red);
        break;
    case 2:
        zd->changeColor(Qt::green);
        break;
    case 3:
        zd->changeColor(Qt::blue);
        break;
    case 4:
        zd->changeColor(Qt::white);
        break;
    case 5:
        zd->changeColor(Qt::darkCyan);
        break;
    case 6:
        zd->changeColor(Qt::yellow);
        break;
    case 7:
        zd->changeColor(Qt::magenta);
        break;
    case 8:
        zd->changeColor(Qt::gray);
        break;
    }
}

void MainWindow::on_widthSpinBox_valueChanged(int value) {
    zd->changeWidth(value);
}

void MainWindow::on_solidStyleRadioButton_clicked() {
    zd->changeStyle(Qt::SolidLine);
}

void MainWindow::on_dashedStyleRadioButton_clicked() {
    zd->changeStyle(Qt::DashLine);
}

void MainWindow::on_dottedStyleRadioButton_clicked() {
    zd->changeStyle(Qt::DotLine);
}

void MainWindow::on_dashDotStyleRadioButton_clicked() {
    zd->changeStyle(Qt::DashDotLine);
}

void MainWindow::changeTool(QAction *sender) {
    if (sender == drawToolAction) {
        MESSAGE("Outil de dessin choisi");
        zd->setCursor(Qt::CrossCursor);
        zd->changeTool(Tool::Draw);
        ui->moveToolButton->setEnabled(false);
        ui->eraseToolButton->setEnabled(false);
        ui->formComboBox->setEnabled(true);
        ui->colorComboBox->setEnabled(true);
        ui->widthSpinBox->setEnabled(true);
        ui->styleGroupBox->setEnabled(true);
        ui->drawOptions->setEnabled(true);
    }
    else if (sender == selectToolAction) {
        MESSAGE("Outil de selection choisi");
        zd->setCursor(Qt::ArrowCursor);
        zd->changeTool(Tool::Select);
        ui->moveToolButton->setEnabled(false);
        ui->eraseToolButton->setEnabled(false);
        ui->formComboBox->setEnabled(false);
        ui->colorComboBox->setEnabled(false);
        ui->widthSpinBox->setEnabled(false);
        ui->styleGroupBox->setEnabled(false);
    }
    else if (sender ==  moveToolAction) {
        MESSAGE("Outil de movimentation choisi");
        zd->setCursor(Qt::SizeAllCursor);
        zd->changeTool(Tool::Move);
        ui->drawOptions->setEnabled(false);
    }
    else if (sender == eraseToolAction) {
        MESSAGE("Outil de éffacement choisi");
        ui->moveToolButton->setEnabled(false);
    }
    else {
        MESSAGE("[ERREUR] Cet outil n'est pas défini!");
    }
}

void MainWindow::closeEvent(QCloseEvent *e) {
    if (zd->hasChanged()) {
        QMessageBox mb;

        mb.setText("Zone de Dessin changed!");
        mb.setInformativeText("Would you like to save your modifications?");
        mb.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        mb.setDefaultButton(QMessageBox::Save);

        int ret = mb.exec();
        if (ret == QMessageBox::Save)
            save();
        else if (ret == QMessageBox::Discard)
            e->accept();
        else if (ret == QMessageBox::Cancel)
            e->ignore();
    }
}

void MainWindow::onDessinSelected() {
    if (zd->currentTool() == Tool::Select) {
        ui->moveToolButton->setEnabled(true);
        ui->eraseToolButton->setEnabled(true);
        ui->colorComboBox->setEnabled(true);
        ui->widthSpinBox->setEnabled(true);
        ui->styleGroupBox->setEnabled(true);
    }
}

void MainWindow::onDessinUnselected() {
    if (zd->currentTool() == Tool::Select) {
        ui->moveToolButton->setEnabled(false);
        ui->eraseToolButton->setEnabled(false);
        ui->colorComboBox->setEnabled(false);
        ui->widthSpinBox->setEnabled(false);
        ui->styleGroupBox->setEnabled(false);
    }
}
