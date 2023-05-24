#include "terraingentabcontrol.h"

#include "heightmapeditortab.h"
#include "terraintab.h"

TerrainGenTabControl::TerrainGenTabControl(QWidget* parent)
    : QTabWidget{parent}
{
    HeightmapEditorTab* node_tab = findChild<HeightmapEditorTab*>();
    TerrainTab* terrain_tab = findChild<TerrainTab*>();

    connect(node_tab, &HeightmapEditorTab::HeightmapChanged,
            terrain_tab, &TerrainTab::OnHeightmapChanged);

    node_tab = nullptr;
    terrain_tab = nullptr;
}
