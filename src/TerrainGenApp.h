#include "TGWindow.h"

#include <wx/wx.h>

class TerrainGenApp : public wxApp {

public:
	virtual bool OnInit();

private:
	TGWindow* mainWindow;
};
