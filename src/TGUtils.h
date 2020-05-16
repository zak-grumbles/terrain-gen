#ifndef TG_UTILS_H
#define TG_UTILS_H

#include <string>
#include <vector>

// Static class containing some utility functions
class TGUtils {

public:
	static std::string read_file(const std::string& filepath);

	static std::vector<char> read_bytes(const std::string& filepath);
};

#endif