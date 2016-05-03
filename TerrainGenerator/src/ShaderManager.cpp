#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include "ShaderManager.h"

using namespace std;

ShaderManager::ShaderManager(){
	
}

ShaderManager::~ShaderManager(){
	glDetachShader(program, vertShaderId);
	glDetachShader(program, fragShaderId);

	glDeleteShader(vertShaderId);
	glDeleteShader(fragShaderId);

	glDeleteProgram(program);
}

void ShaderManager::loadTexture(std::string texPath, std::string bumpPath){
	texture = SOIL_load_OGL_texture(texPath.c_str(), SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y |
		SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

	bump = SOIL_load_OGL_texture(bumpPath.c_str(), SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y |
		SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
}

void ShaderManager::loadFile(const char* path, string& str){
	ifstream f(path);

	if (!f.is_open()){
		printf("Unable to open shader %s.\n", path);
		return;
	}

	const int lineSize = 300;
	char buffer[lineSize];
	while (!f.eof()){
		f.getline(buffer, lineSize);
		str += buffer;
		str += '\n';
	}
}

unsigned int ShaderManager::loadShader(string& src, unsigned int mode){
	unsigned int id;
	GLenum err = glewInit();
	id = glCreateShader(mode);

	const char* c_src = src.c_str();

	glShaderSource(id, 1, &c_src, NULL);
	glCompileShader(id);
	char error[1024];
	glGetShaderInfoLog(id, 1024, NULL, error);
	printf("%s\n", error);

	return id;
}

GLint ShaderManager::initShader(const char* vertPath, const char* fragPath){
	string src;

	loadFile(vertPath, src);
	vertShaderId = loadShader(src, GL_VERTEX_SHADER);

	src = "";
	loadFile(fragPath, src);
	fragShaderId = loadShader(src, GL_FRAGMENT_SHADER);

	program = glCreateProgram();
	glAttachShader(program, vertShaderId);
	glAttachShader(program, fragShaderId);

	glLinkProgram(program);

	return program;
}