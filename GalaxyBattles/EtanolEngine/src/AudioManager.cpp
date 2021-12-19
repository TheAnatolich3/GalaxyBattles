#include <stdexcept>
#include <iostream>
#include <SDL.h>
#include "AudioManager.hpp"

void AudioManager::audio_callback(void* userdata, uint8_t* stream, int len)
{
	auto audioManager = static_cast<AudioManager*>(userdata);
	SDL_memset(stream, 0, len);
	audioManager->_lock_buffer.lock();
	for (auto& buffer : audioManager->_buffers)
	{
		auto sound = buffer.lock();
		if (sound != nullptr)
		{
			if (sound->is_playing())
			{
				auto amount = sound->_len_file - sound->_pos;
				if (amount > len)
				{
					amount = len;
				}

				SDL_MixAudioFormat(stream,
					sound->_data + sound->_pos,
					AUDIO_S16LSB,
					amount,
					sound->_volume);

				sound->_pos += amount;

				if (sound->_pos >= sound->_len_file)
				{
					if (sound->_isLoop)
					{
						sound->_pos = 0;
					}
					else
					{
						sound->stop();
					}
				}
			}
		}
	}
	audioManager->_lock_buffer.unlock();
}

AudioManager::AudioManager()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_memset(&_wanted_spec, 0, sizeof(_wanted_spec));
	_wanted_spec.freq = 44100;
	_wanted_spec.format = AUDIO_S16LSB;
	_wanted_spec.channels = 2;
	_wanted_spec.samples = 4096;
	_wanted_spec.callback = AudioManager::audio_callback;
	_wanted_spec.userdata = this;

	SDL_AudioSpec returned{};
	const int32_t allow_changes = 0;
	const char* device_name = nullptr;

	_audio_device = SDL_OpenAudioDevice(
		device_name,
		0,
		&_wanted_spec,
		&returned,
		allow_changes);

	if (_audio_device == 0)
	{
		throw std::runtime_error("Can't open Audio Device");
	}

	if (_wanted_spec.format != returned.format
		|| _wanted_spec.channels != returned.channels
		|| _wanted_spec.freq != returned.freq)
	{
		throw std::runtime_error("Audio Device doesn't support our format");
	}
}

std::shared_ptr<Sound> AudioManager::createSound(std::string_view file_name, bool is_loop, int volume) const
{
	std::shared_ptr<Sound> sound = std::make_shared<Sound>(file_name, is_loop, volume);
	_buffers.push_back(sound);
	return sound;
}


void AudioManager::update()
{
	_lock_buffer.lock();
	_buffers.erase(std::remove_if(_buffers.begin(), _buffers.end(), [](const std::weak_ptr<Sound>& s) {
		return s.expired(); }), _buffers.end());
	_lock_buffer.unlock();
	if (_buffers.size() != 0)
	{
		bool is_play = false;
		for (auto buffer : _buffers)
		{
			auto sound = buffer.lock();
			if (sound->is_playing())
			{
				is_play = true;
				break;
			}
		}
		if (is_play)
		{
			SDL_PauseAudioDevice(_audio_device, SDL_FALSE);
		}
		else
		{
			SDL_PauseAudioDevice(_audio_device, SDL_TRUE);
		}
	}
	else
	{
		SDL_PauseAudioDevice(_audio_device, SDL_TRUE);
	}
}

