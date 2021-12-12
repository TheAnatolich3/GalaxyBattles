#include <stdexcept>
#include <iostream>
#include "AudioManager.hpp"


void AudioManager::audio_callback(void* userdata, uint8_t* stream, int len)
{
	auto audioManager = static_cast<AudioManager*>(userdata);
	SDL_memset(stream, 0, len);
	for (auto& buffer : audioManager->_buffers)
	{
		auto amount = buffer.lock()->_len_file - buffer.lock()->_pos;
		if (amount > len)
		{
			amount = len;
		}

		SDL_MixAudioFormat(stream,
			*(buffer.lock()->_data) + buffer.lock()->_pos,
			AUDIO_S16LSB,
			amount,
			buffer.lock()->_volume);

		buffer.lock()->_pos += amount;

		if (buffer.lock()->_pos >= buffer.lock()->_len_file)
		{
			if (buffer.lock()->_isLoop)
			{
				buffer.lock()->_pos = 0;
			}
			else
			{
				buffer.lock()->_state = Sound::State::ST_END;
			}
		}
	}
}

std::shared_ptr<Sound> AudioManager::createSound(std::string_view file_name, bool is_loop, int volume)
{
	std::shared_ptr<Sound> sound = std::make_shared<Sound>(file_name, is_loop, volume);
	_audio_spec_from_file = sound->get_AudioFormat();
	_audio_spec_from_file.callback = this->audio_callback;
	_audio_spec_from_file.userdata = this;
	_audio_spec_from_file.channels = 2;

	SDL_AudioSpec returned{};
	const int32_t allow_changes = 0;
	const char* device_name = nullptr;
	int i, count = SDL_GetNumAudioDevices(0);


	_audio_device = SDL_OpenAudioDevice(
		device_name,
		0,
		&_audio_spec_from_file,
		&returned,
		allow_changes);

	if (_audio_device == 0)
	{
		throw std::runtime_error("Can't open Audio Device");
	}

	if (_audio_spec_from_file.format != returned.format
		|| _audio_spec_from_file.channels != returned.channels
		|| _audio_spec_from_file.freq != returned.freq)
	{
		throw std::runtime_error("Audio Device doesn't support our format");
	}

	_buffers.push_back(sound);
	return sound;
}


void AudioManager::update()
{
	_buffers.erase(std::remove_if(_buffers.begin(), _buffers.end(), [](const std::weak_ptr<Sound>& s) {
		return s.lock()->get_state() == Sound::State::ST_END; }), _buffers.end());

	if (_buffers.size() != 0)
	{
		bool is_play = false;
		for (auto& sound : _buffers)
		{
			if (sound.lock()->is_playing())
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

