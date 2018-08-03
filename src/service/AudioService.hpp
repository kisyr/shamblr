#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <sndfile.h>

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
			if (!alcMakeContextCurrent(m_context)) {
				throw std::runtime_error("alcMakeContextCurrent");
			}

			const ALfloat orientation[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
			alListener3f(AL_POSITION, 0.0f, 0.0f, 1.0f);
			alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
			alListenerfv(AL_ORIENTATION, orientation);

			ALuint source;
			alGenSources(1, &source);
			alSourcef(source, AL_PITCH, 1.0f);
			alSourcef(source, AL_GAIN, 1.0f);
			alSource3f(source, AL_POSITION, 0.0f, 0.0f, 0.0f);
			alSource3f(source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
			alSourcei(source, AL_LOOPING, AL_FALSE);

			m_sources["default"] = source;
		}

		~AudioService() {
			alcDestroyContext(m_context);
			alcCloseDevice(m_device);
		}

		void createSound(const std::string& name, const std::string& path) {
			SF_INFO info = {0};
			SNDFILE* file = sf_open(path.c_str(), SFM_READ, &info);

			SHAMBLR_LOG("Loaded %s (frames: %ld, samplerate: %d, channels: %d, format: %d, sections: %d, seekable: %d)\n",
				path.c_str(), info.frames, info.samplerate, info.channels, info.format, info.sections, info.seekable
			);

			std::vector<short> data;
			data.resize(info.frames * info.channels);
			auto readCount = sf_readf_short(file, &data[0], info.frames);

			sf_close(file);

			ALuint buffer;
			alGenBuffers(1, &buffer);
			alBufferData(buffer, AL_FORMAT_STEREO16, &data[0], data.size() * sizeof(short), info.samplerate);

			m_buffers[name] = buffer;
		}

		void playSound(const std::string& name) {
			const auto source = m_sources["default"];
			const auto buffer = m_buffers[name];

			SHAMBLR_LOG("Playing %s (%d, %d)\n", name.c_str(), source, buffer);

			alSourcei(source, AL_BUFFER, buffer);
			alSourcePlay(source);
		}

	private:
		ALCdevice* m_device;
		ALCcontext* m_context;
		std::unordered_map<std::string, ALuint> m_sources;
		std::unordered_map<std::string, ALuint> m_buffers;
};

} // namespace shamblr
