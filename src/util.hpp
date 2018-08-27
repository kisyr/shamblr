#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

namespace shamblr {
namespace util {

inline size_t generateTriVertices(
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

inline size_t generateRectVertices(
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

inline size_t generateBoxVertices(
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
#if 0
inline float angleBetween(
	glm::quat q1,
	glm::quat q2
) {
	float cosTheta = glm::dot(q1, q2);
	
	// q1 and q2 are already equal.
	// Force q2 just to be sure
	if(cosTheta > 0.9999f){
		return 0.0f;
	}
	
	// Avoid taking the long path around the sphere
	if (cosTheta < 0){
		q1 = q1*-1.0f;
		cosTheta *= -1.0f;
	}
	
	return glm::acos(cosTheta);
}
#endif
inline float angleBetween(
	const glm::quat& x,
	const glm::quat& y
) {
	glm::quat z = y;

	float cosTheta = glm::dot(x, y);

	// If cosTheta < 0, the interpolation will take the long way around the sphere. 
	// To fix this, one quat must be negated.
	if (cosTheta < float(0)) {
		z        = -y;
		cosTheta = -cosTheta;
	}

	// Perform a linear interpolation when cosTheta is close to 1 to avoid side effect of sin(angle) becoming a zero denominator
	if(cosTheta > float(1) - glm::epsilon<float>()) {
		return float(0);
	}

	// Essential Mathematics, page 467
	float angle = glm::acos(cosTheta);

	return angle;
}

inline glm::quat rotateTowards(
	glm::quat q1,
	glm::quat q2,
	float maxAngle
){
	if( maxAngle < 0.001f ){
		// No rotation allowed. Prevent dividing by 0 later.
		return q1;
	}
	
	float cosTheta = glm::dot(q1, q2);
	
	// q1 and q2 are already equal.
	// Force q2 just to be sure
	if(cosTheta > 0.9999f){
		return q2;
	}
	
	// Avoid taking the long path around the sphere
	if (cosTheta < 0){
		q1 = q1*-1.0f;
		cosTheta *= -1.0f;
	}
	
	float angle = glm::acos(cosTheta);
	
	// If there is only a 2° difference, and we are allowed 5°,
	// then we arrived.
	if (angle < maxAngle){
		return q2;
	}

	// This is just like slerp(), but with a custom t
	float t = maxAngle / angle;
	angle = maxAngle;
	
	glm::quat res = (glm::sin((1.0f - t) * angle) * q1 + glm::sin(t * angle) * q2) / glm::sin(angle);
	res = glm::normalize(res);
	return res;
}

} // namespace util
} // namespace shamblr
