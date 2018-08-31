#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <json.hpp>
#include <stb_image.h>

namespace shamblr {

inline std::string loadFile(const std::string& path) {
	std::ifstream file(path);
	if (file.fail()) {
		throw std::runtime_error(SHAMBLR_FMT("loadFile: %s", path));
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

using Json = nlohmann::json;

inline Json loadJson(const std::string& path) {
	return Json::parse(loadFile(path).c_str());
}

struct Image {
	int width;
	int height;
	int bpp;
	std::vector<unsigned char> pixels;
};

inline bool loadImage(const std::string& path, Image& image) {
	unsigned char* pixels = stbi_load(
		path.c_str(),
		&image.width,
		&image.height,
		&image.bpp,
		0
	);
	if (!pixels) {
		throw std::runtime_error(SHAMBLR_FMT("loadImage: %s", stbi_failure_reason()));
	}
	image.pixels.resize(image.width * image.height * image.bpp);
	memcpy(
		image.pixels.data(),
		pixels,
		sizeof(GLubyte) * image.width * image.height * image.bpp
	);
	stbi_image_free(pixels);
	return true;
}

} // namesoace shamblr
