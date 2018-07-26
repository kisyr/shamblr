#pragma once

#include "common.hpp"

namespace shamblr {
namespace events {

struct Attack {
	Entity origin;
	Entity target;
};

} // namespace events
} // namespace shamblr
