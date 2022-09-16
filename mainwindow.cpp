#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusbar->showMessage("Staring up. . .");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setStatusMessage(const QString& msg)
{
    ui->statusbar->showMessage(msg);
}

