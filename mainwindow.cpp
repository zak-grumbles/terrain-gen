#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);
    ui_->statusbar->showMessage("Starting up. . .");
}

MainWindow::~MainWindow()
{
    delete ui_;
}

void MainWindow::SetStatusMessage(const QString& msg)
{
    ui_->statusbar->showMessage(msg);
}

