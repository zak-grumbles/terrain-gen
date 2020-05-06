#include "TGWindow.h"

wxBEGIN_EVENT_TABLE(TGWindow, wxFrame)
EVT_MENU(wxID_EXIT, TGWindow::OnExit)
wxEND_EVENT_TABLE()

TGWindow::TGWindow(
	const wxString& title,
	const wxPoint& pos,
	const wxSize& size
) : wxFrame(nullptr, wxID_ANY, title, pos, size)
{
	wxMenu* fileMenu = new wxMenu;
	fileMenu->Append(wxID_EXIT);

	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(fileMenu, "&File");

	SetMenuBar(menuBar);
}

void TGWindow::OnExit(wxCommandEvent& e)
{
	Close(true);
}
