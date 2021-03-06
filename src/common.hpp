#pragma once

#include <array>
#include <chrono>
#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <entt/entt.hpp>
#include <tinyformat.h>

#define SHAMBLR_LOG(f_, ...) printf("%s", tfm::format(f_, ##__VA_ARGS__).c_str())
#define SHAMBLR_FMT tfm::format

namespace shamblr {

typedef entt::DefaultRegistry EntityRegistry;
typedef EntityRegistry::entity_type Entity;
typedef entt::Dispatcher EventDispatcher;

struct Time {
	float elapsed;
	float delta;
};

template <class T>
std::shared_ptr<T> locateService() {
	auto service = entt::ServiceLocator<T>::get();
	auto pointer = service.lock();
	if (!pointer) {
		throw std::runtime_error("Invalid service pointer");
	}
	return pointer;
}

template <class T, class... Args>
std::shared_ptr<T> registerService(Args... args) {
	entt::ServiceLocator<T>::set(args...);
	return locateService<T>();
}

} // namespace shamblr
