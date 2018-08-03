#pragma once

#include <AL/al.h>
#include <AL/alc.h>

namespace shamblr {

class AudioService {
	public:
		AudioService() : m_device(NULL) {
			m_device = alcOpenDevice(NULL);
			if (!m_device) {
				throw std::runtime_error("alcOpenDevice");
			}
			m_context = alcCreateContext(m_device, NULL);
			if (!m_device) {
				throw std::runtime_error("alcCreateContext");
			}
		}

		~AudioService() {}

		void createSound(const std::string& name, const std::string& path) {}
		void playSound(const std::string& name) {}

	private:
		ALCdevice* m_device;
		
};

} // namespace shamblr
