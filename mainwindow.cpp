#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "widgets/heightmapeditortab.h"
#include "widgets/terrainviewwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);
    ui_->statusbar->showMessage("Starting up. . .");

    HeightmapEditorTab* node_view = findChild<HeightmapEditorTab*>();
    TerrainViewWidget* terrain_view = findChild<TerrainViewWidget*>();

    if(node_view != nullptr && terrain_view != nullptr)
    {
        connect(node_view, &HeightmapEditorTab::HeightmapChanged,
                terrain_view, &TerrainViewWidget::OnHeightmapUpdated);
    }
    else
    {
        qWarning() << "CHILDREN NOT FOUND";
    }
}

MainWindow::~MainWindow()
{
    delete ui_;
}

void MainWindow::SetStatusMessage(const QString& msg)
{
    ui_->statusbar->showMessage(msg);
}

