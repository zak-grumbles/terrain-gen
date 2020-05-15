#include "TGUtils.h"

#include <fstream>

std::string TGUtils::read_file(const std::string& filepath) {

	std::ifstream file(filepath, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		std::string err = "Unable to open file '" + filepath + "'";
		throw std::runtime_error(err);
	}

	size_t size = (size_t)file.tellg();
	std::string buf;
	buf.reserve(size);
	file.seekg(0);

	buf.assign((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());

	file.close();

	return buf;
}