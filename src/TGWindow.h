#ifndef TG_VULKAN_WINDOW_H
#define TG_VULKAN_WINDOW_H

#include <wx/wx.h>

class TGWindow : public wxFrame 
{
public:
	TGWindow(
		const wxString& title,
		const wxPoint& point,
		const wxSize& size
	);

private:
	void OnExit(wxCommandEvent& e);

	wxDECLARE_EVENT_TABLE();
};

#endif