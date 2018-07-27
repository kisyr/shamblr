#pragma once

#include "common.hpp"

namespace shamblr {
namespace events {

struct Attack {
	Entity origin;
	Entity target;
};

struct ProjectileHit {
	Entity victim;
};

} // namespace events
} // namespace shamblr
