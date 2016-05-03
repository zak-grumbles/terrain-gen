#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <string>

class ShaderManager{
public:

	ShaderManager();
	~ShaderManager();

	void loadTexture(std::string texPath, std::string bumpPath);
	void loadFile(const char* path, std::string& str);

	unsigned int loadShader(std::string& src, unsigned int mode);

	GLint initShader(const char* vertPath, const char* fragPath);

	unsigned int layer1ID;
	unsigned int layer2ID;

	unsigned int program;

private:
	unsigned int vertShaderId;
	unsigned int fragShaderId;

	GLuint texture;
	GLuint bump;
};

#endif