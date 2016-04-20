#include "Renderer.h"

int monitor_width;
int monitor_height;
int window_width = 640;
int window_height = 480;
int main_window;

int main(int argc, char* argv[]){
	Renderer::init(window_width, window_height, 45.0f, argc, argv);

	std::vector<gmtl::Point3f> v = std::vector<gmtl::Point3f>();
	v.push_back(gmtl::Point3f(-0.5, 0.0, -3.0));
	v.push_back(gmtl::Point3f(0.0, 0.5, -3.0));
	v.push_back(gmtl::Point3f(0.5, 0.0, -3.0));

	Renderer::setVertices(v);

	Renderer::start();

	return 0;
}