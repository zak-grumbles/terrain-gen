#ifndef TG_GENERATOR_H
#define TG_GENERATOR_H

#include <wx/wx.h>

#include "FastNoise/FastNoise.h"
#include "TGTypes.h"
#include <glm/glm.hpp>

#include <memory>
#include <vector>

wxDECLARE_EVENT(wxEVT_TG_GENERATOR_PROGRESS, wxCommandEvent);
wxDECLARE_EVENT(wxEVT_TG_GENERATOR_DONE, wxCommandEvent);

struct TGGeneratorRange {
	float min_x;
	float max_x;
	float min_z;
	float max_z;
};

class TGTerrainData : public wxClientData {
private:
	std::unique_ptr<std::vector<Vertex>> vertices_;

public:
	TGTerrainData(std::unique_ptr<std::vector<Vertex>> verts);
	std::unique_ptr<std::vector<Vertex>> take_vertices() noexcept;
};

class TGGenerator : public wxThread {

private:
	int num_cells_;
	float cell_size_;
	float target_;

	glm::vec3 origin_;

	TGGeneratorRange range_;

	std::unique_ptr<std::vector<Vertex>> vertices_;

	FastNoise noise_generator_;

	wxEvtHandler* parent_;

public:
	TGGenerator(wxEvtHandler* parent, int num_cells, float cell_size);
	wxThread::ExitCode Entry();

private:
	void march_cube(glm::vec3 cube_origin);

	float get_offset(float a, float b);
};

#endif