#include "mainwindow.h"

#include <QActionGroup>
#include <QFileDialog>
#include <QMenuBar>

#include "./ui_mainwindow.h"
#include "widgets/heightmapeditortab.h"
#include "widgets/terrainviewwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow) {
    ui_->setupUi(this);

    node_view_    = findChild<HeightmapEditorTab *>();
    terrain_view_ = findChild<TerrainViewWidget *>();

    if (node_view_ != nullptr && terrain_view_ != nullptr) {
        connect(
            node_view_, &HeightmapEditorTab::HeightmapChanged, terrain_view_,
            &TerrainViewWidget::OnHeightmapUpdated
        );
    } else {
        qWarning() << "CHILDREN NOT FOUND";
    }

    CreateActions_();
    CreateMenus_();
}

MainWindow::~MainWindow() {
    delete ui_;
}

void MainWindow::ExportHeightmap() {
    QString filename = QFileDialog::getSaveFileName(
        this, tr("Export heightmap"), "",
        tr("Image (*.bmp,*.png);;All Files(*)")
    );

    if (!filename.isEmpty()) {
        auto heightmap = node_view_->GetHeightmap();
        QFile dest(filename);
        if (heightmap != nullptr) {
            if (dest.open(QIODevice::WriteOnly)) {
                auto heightmap_img = heightmap->toImage();
                heightmap->save(&dest);
            } else {
                // TODO: Error
            }
        } else {
            // TODO: Error
        }
    } else {
        // TODO: Error
    }
}

void MainWindow::CreateActions_() {
    export_heightmap_ = new QAction(tr("&Export Heightmap"), this);
    export_heightmap_->setStatusTip(tr("Export the created heightmap"));
    connect(
        export_heightmap_, &QAction::triggered, this,
        &MainWindow::ExportHeightmap
    );

    heightmap_group_ = new QActionGroup(this);
    heightmap_group_->addAction(export_heightmap_);
}

void MainWindow::CreateMenus_() {
    file_menu_ = menuBar()->addMenu(tr("&File"));
    file_menu_->addAction(export_heightmap_);
}
