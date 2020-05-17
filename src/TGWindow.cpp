#include "TGWindow.h"

wxBEGIN_EVENT_TABLE(TGWindow, wxFrame)
EVT_MENU(wxID_EXIT, TGWindow::on_exit)
EVT_CLOSE(TGWindow::on_close)
wxEND_EVENT_TABLE()

TGWindow::TGWindow(
	const wxString& title,
	const wxPoint& pos,
	const wxSize& size
) : wxFrame(nullptr, wxID_ANY, title, pos, size) {

	wxMenu* file_menu = new wxMenu;
	file_menu->Append(wxID_EXIT);

	wxMenuBar* menu_bar = new wxMenuBar;
	menu_bar->Append(file_menu, "&File");

	SetMenuBar(menu_bar);

	canvas_ = new TGVulkanCanvas(this, wxID_ANY, wxDefaultPosition, { 800, 600 });
	Fit();
}

void TGWindow::on_exit(wxCommandEvent& e) {
	Close(true);
}

void TGWindow::on_close(wxCloseEvent& e) {
	if (canvas_) {
		canvas_->terminate();
		delete canvas_;
	}
	e.Skip();
}

