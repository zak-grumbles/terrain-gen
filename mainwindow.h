#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "widgets/heightmapeditortab.h"
#include "widgets/terrainviewwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

private slots:
    void ExportHeightmap();

private:
    void CreateActions_();
    void CreateMenus_();

    Ui::MainWindow *ui_;

    HeightmapEditorTab* node_view_ = nullptr;
    TerrainViewWidget* terrain_view_ = nullptr;

    QMenu* file_menu_ = nullptr;

    QActionGroup* heightmap_group_ = nullptr;
    QAction* export_heightmap_ = nullptr;
};
#endif // MAINWINDOW_H
