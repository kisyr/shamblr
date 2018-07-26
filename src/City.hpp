#pragma once

#include <glm/gtc/random.hpp>
#include <vector>
#include <algorithm>

namespace shamblr {

class Rectangle {
	public:
		Rectangle(const glm::vec2& min, const glm::vec2& max) :
				m_min(min),
				m_max(max) {}

		glm::vec2 size() const {
			return m_max - m_min;
		}

		float area() const {
			return size().x * size().y;
		}

		glm::vec2 center() const {
			return m_min + size() / 2.0f;
		}

		Rectangle scale(const glm::vec2& amount) const {
			return Rectangle(m_min + (amount  * 0.5f), m_max - (amount * 0.5f));
		}

		std::pair<Rectangle, Rectangle> divide(
			const glm::ivec2& axis,
			const float center = 0.5f
		) const {
			const auto a = Rectangle(m_min, m_max - (size() * glm::vec2(axis) * (1.0f - center)));
			const auto b = Rectangle(m_min + (size() * glm::vec2(axis) * center), m_max);
			return std::make_pair(a, b);
		}

		glm::vec2 min() const {
			return m_min;
		}

		glm::vec2 max() const {
			return m_max;
		}

	private:
		glm::vec2 m_min;
		glm::vec2 m_max;
};

class City {
	public:
		void generate(const glm::vec2& size) {
			assert(size.x > 0.0f && size.y > 0.0f);
			m_lots = std::vector<Rectangle>{
				{ -(size / 2.0f), (size / 2.0f) },
			};
			m_roads = std::vector<Rectangle>{};
			subdivideRects(40.0f * 40.0f, 6.0f);
			subdivideRects(25.0f * 25.0f, 3.0f);
			subdivideRects(10.0f * 10.0f, 1.0f);
		}

		const std::vector<Rectangle>& lots() const {
			return m_lots;
		}

		const std::vector<Rectangle>& roads() const {
			return m_roads;
		}

	private:
		void subdivideRects(const float targetArea, const float roadSize) {
			const auto smallestArea = [](const auto& a, const auto& b) {
				return a.area() < b.area();
			};
			while (!m_lots.empty() && m_lots.back().area() > targetArea * 2.0f) {
				// Get the largest block
				const auto block = m_lots.back();
				m_lots.pop_back();
				// Decide which axis to split on
				const glm::ivec2 axis = block.size().x > block.size().y ? 
					glm::ivec2(1,0) : glm::ivec2(0,1);
				// Split twice to get 3 m_lots (second one on the largest split)
				const float amount1 = glm::linearRand(0.25f, 0.5f);
				const auto split1 = block.divide(axis, amount1);
				const float axisSize = (axis.x > axis.y ? split1.second.size().x : split1.second.size().y);
				const float amount2 = roadSize / axisSize;
				const auto split2 = split1.second.divide(axis, amount2);
				// Push the first and last block to building lots
				m_lots.push_back(split1.first);
				m_lots.push_back(split2.second);
				// Push the middle block to roads
				m_roads.push_back(split2.first);
				// Sort m_lots small to large
				std::sort(m_lots.begin(), m_lots.end(), smallestArea);
			}
		}

	std::vector<Rectangle> m_lots;
	std::vector<Rectangle> m_roads;
};

} // namespace shamblr
