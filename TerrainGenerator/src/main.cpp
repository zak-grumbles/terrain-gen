#include "Renderer.h"
#include "TerrainGenerator.h"

int monitor_width;
int monitor_height;
int window_width = 640;
int window_height = 480;
float view_angle = 45.0f;

void displayFunction(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glLoadIdentity();
    gluPerspective(view_angle, (float)window_width/ (float)window_height, 0.1, 30);


    Renderer::draw();
    glutSwapBuffers();
}

int main(int argc, char* argv[]){
	Renderer::init(window_width, window_height, 45.0f, argc, argv);

    /*Test triangle to make sure everything is working
	std::vector<gmtl::Point3f> v = std::vector<gmtl::Point3f>();
	v.push_back(gmtl::Point3f(-0.5, 0.0, -3.0));
	v.push_back(gmtl::Point3f(0.0, 0.5, -3.0));
	v.push_back(gmtl::Point3f(0.5, 0.0, -3.0));
	*/

	TerrainGenerator *tg = new TerrainGenerator(50, 50, 50);
	std::vector<gmtl::Point3f> verts = tg->getVerts();

	Renderer::setVertices(verts);
    Renderer::setDisplayFunction(displayFunction);
	
    Renderer::start();
	return 0;
}