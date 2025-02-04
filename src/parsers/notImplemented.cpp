#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<char> readGIF(const std::string& filename) {
	std::ifstream file(filename.c_str(), std::ios::binary);
	if (!file) {
		std::cerr << "Error opening file: " << filename << std::endl;
		//return std::vector<char>();
	}

	// Seek to the end to get size
	file.seekg(0, std::ios::end);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	// Read contents
	std::vector<char> buffer(size);
	if (!file.read(buffer.data(), size)) {
		std::cerr << "Error reading file: " << filename << std::endl;
		return std::vector<char>();
	}

	return buffer;
}

