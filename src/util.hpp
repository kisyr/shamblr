#pragma once

#include <glm/glm.hpp>

namespace shamblr {
namespace util {

size_t generateTriVertices(
	const glm::vec3& min,
	const glm::vec3& max,
	std::vector<glm::vec3>& out
) {
	const glm::vec3 vertices[] = {
		{ min.x, min.y, min.z },
		{ (max.x - min.x) / 2, min.y, max.z },
		{ max.x, min.y, min.z },
	};
	const auto count = sizeof(vertices) / sizeof(vertices[0]);
	out.insert(out.end(), vertices, vertices + count);
	return count;
}

size_t generateRectVertices(
	const glm::vec3& min,
	const glm::vec3& max,
	std::vector<glm::vec3>& out
) {
	const glm::vec3 vertices[] = {
		{ min.x, min.y, min.z },
		{ min.x, min.y, max.z },
		{ max.x, min.y, min.z },
		{ min.x, min.y, max.z },
		{ max.x, min.y, max.z },
		{ max.x, min.y, min.z },
	};
	const auto count = sizeof(vertices) / sizeof(vertices[0]);
	out.insert(out.end(), vertices, vertices + count);
	return count;
}

size_t generateBoxVertices(
	const glm::vec3& min,
	const glm::vec3& max,
	std::vector<glm::vec3>& out
) {
	const glm::vec3 vertices[] = {
		// Bottom
		{ min.x, min.y, min.z },
		{ min.x, min.y, max.z },
		{ max.x, min.y, min.z },
		{ min.x, min.y, max.z },
		{ max.x, min.y, max.z },
		{ max.x, min.y, min.z },
		// Top
		{ min.x, max.y, min.z },
		{ min.x, max.y, max.z },
		{ max.x, max.y, min.z },
		{ min.x, max.y, max.z },
		{ max.x, max.y, max.z },
		{ max.x, max.y, min.z },
		// West
		{ min.x, min.y, min.z },
		{ min.x, min.y, max.z },
		{ min.x, max.y, min.z },
		{ min.x, min.y, max.z },
		{ min.x, max.y, max.z },
		{ min.x, max.y, min.z },
		// East
		{ max.x, min.y, min.z },
		{ max.x, min.y, max.z },
		{ max.x, max.y, min.z },
		{ max.x, min.y, max.z },
		{ max.x, max.y, max.z },
		{ max.x, max.y, min.z },
		// South
		{ min.x, min.y, min.z },
		{ max.x, min.y, min.z },
		{ min.x, max.y, min.z },
		{ max.x, min.y, min.z },
		{ max.x, max.y, min.z },
		{ min.x, max.y, min.z },
		// North
		{ min.x, min.y, max.z },
		{ max.x, min.y, max.z },
		{ min.x, max.y, max.z },
		{ max.x, min.y, max.z },
		{ max.x, max.y, max.z },
		{ min.x, max.y, max.z },
	};
	const auto count = sizeof(vertices) / sizeof(vertices[0]);
	out.insert(out.end(), vertices, vertices + count);
	return count;
}

} // namespace util
} // namespace shamblr
