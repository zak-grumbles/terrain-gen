#ifndef TG_VULKAN_WINDOW_H
#define TG_VULKAN_WINDOW_H

#include <wx/wx.h>

#include "TGVulkanCanvas.h"

class TGWindow : public wxFrame 
{
private:
	TGVulkanCanvas* canvas_;

public:
	TGWindow( const wxString& title, const wxPoint& point, const wxSize& size );

private:
	void OnResize(wxSizeEvent& e);
	void OnExit(wxCommandEvent& e);

	wxDECLARE_EVENT_TABLE();
};

#endif