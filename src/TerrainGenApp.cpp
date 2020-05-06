#include "TerrainGenApp.h"

bool TerrainGenApp::OnInit()
{
	mainWindow = new TGWindow("TerrainGen", wxPoint(50, 50), wxSize(1280, 960));
	mainWindow->Show(true);
	return true;
}
