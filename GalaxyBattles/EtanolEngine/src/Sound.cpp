#include <stdexcept>
#include <vector>
#include <string>
#include "Sound.hpp"

namespace
{
	struct AudioBuf
	{
		uint8_t* start = nullptr;
		size_t size = 0;
		size_t current_pos = 0;
	};

	struct AudioManager
	{
		std::vector<AudioBuf> buffers;
	};

	static void audio_callback(void* userdata, uint8_t* stream, int len)
	{
		auto audioManager = static_cast<AudioManager*>(userdata);
		SDL_memset(stream, 0, len);
		for (auto& buffer : audioManager->buffers)
		{
			auto amount = buffer.size - buffer.current_pos;
			if (amount > len)
			{
				amount = len;
			}
			
			SDL_MixAudioFormat(stream,
				buffer.start + buffer.current_pos,
				AUDIO_S16LSB,
				amount,
				SDL_MIX_MAXVOLUME);

			buffer.current_pos += amount;

			if (buffer.current_pos >= buffer.size)
			{
				buffer.current_pos = 0;
			}
			
		}
		
	}
}


Sound::Sound(std::string_view filename, bool is_loop):
	_isLoop(is_loop)
{
	SDL_RWops* file = SDL_RWFromFile(filename.data(), "rb");

	if (file == nullptr)
	{
		throw std::runtime_error("Opening file error: " + std::string{filename});
	}

	uint8_t* sample_buffer_from_file = nullptr;
	uint32_t sample_buffer_len_from_file = 0;

	SDL_AudioSpec audio_spec_from_file{}, returned{};
	SDL_AudioSpec* audio_spec = SDL_LoadWAV_RW(file, 1, &audio_spec_from_file, &sample_buffer_from_file, &sample_buffer_len_from_file);

	if (audio_spec == nullptr)
	{
		throw std::runtime_error("Sound is not loaded!");
	}

	static AudioManager audioManager;
	AudioBuf buf;
	buf.start = sample_buffer_from_file;
	buf.size = sample_buffer_len_from_file;
	buf.current_pos = 0;

	audioManager.buffers.push_back(buf);
	audio_spec_from_file.callback = audio_callback;
	audio_spec_from_file.userdata = &audioManager;

	_audio_device = SDL_OpenAudioDevice(NULL, 0, &audio_spec_from_file, &returned, 0);
	SDL_PauseAudioDevice(_audio_device, SDL_FALSE);
}


void Sound::play()
{
	SDL_PauseAudioDevice(_audio_device, SDL_FALSE);
}

void Sound::pause()
{
	SDL_PauseAudioDevice(_audio_device, SDL_TRUE);
}