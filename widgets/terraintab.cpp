#include "terraintab.h"

TerrainTab::TerrainTab(QWidget *parent)
    : QWidget{parent}
{

}

void TerrainTab::OnHeightmapChanged(std::shared_ptr<QPixmap> heightmap)
{
    qDebug() << "CONNECTED";
}
