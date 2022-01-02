#include <stdexcept>
#include <vector>
#include <string>
#include "Sound.hpp"


Sound::~Sound()
{
	SDL_FreeWAV(_data);
}

Sound::Sound(std::string_view filename, bool is_loop, float volume):
	_isLoop(is_loop), _volume(volume)
{
	SDL_RWops* file = SDL_RWFromFile(filename.data(), "rb");

	if (file == nullptr)
	{
		throw std::runtime_error("Opening file error: " + std::string{filename});
	}

	const int32_t auto_delete_file = 1;
	uint32_t file_size = 0;

	SDL_AudioSpec* audio_spec = SDL_LoadWAV_RW(
		file,
		auto_delete_file,
		&_audio_spec_from_file, 
		&_data,
		&file_size);

	_len_file = file_size;

	if (audio_spec == nullptr)
	{
		throw std::runtime_error("Sound is not loaded!");
	}

	_state = State::ST_STOP;
}


void Sound::play()
{
	_state = State::ST_PLAY;
}

void Sound::pause()
{
	_state = State::ST_PAUSE;
}

void Sound::stop()
{
	_state = State::ST_STOP;
	_pos = 0;
}

bool Sound::is_playing()
{
	return (_state == State::ST_PLAY);
}

SDL_AudioSpec Sound::get_AudioFormat() const
{
	return _audio_spec_from_file;
}

Sound::State Sound::get_state() const
{
	return _state;
}