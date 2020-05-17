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
	void on_resize(wxSizeEvent& e);
	void on_exit(wxCommandEvent& e);
	void on_close(wxCloseEvent& e);

	wxDECLARE_EVENT_TABLE();
};

#endif