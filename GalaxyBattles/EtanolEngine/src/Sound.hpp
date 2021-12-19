#ifndef SOUND_HPP
#define SOUND_HPP
#include <string_view>
#include <memory>
#include <SDL_audio.h>
#include <SDL.h>

class Sound
{
public:
	explicit Sound(std::string_view filename, bool is_loop, int volume);
	~Sound();

	void play();
	void pause();
	void stop();

	bool is_playing();
	friend class AudioManager;
private:
	enum class State {
		ST_PLAY,
		ST_PAUSE,
		ST_STOP
	};

	State _state = State::ST_STOP;
	bool _isLoop = false;
	size_t _len_file = 0;
	size_t _pos = 0;
	uint8_t* _data = nullptr;

	int _volume = 0;

	SDL_AudioSpec _audio_spec_from_file{};
	SDL_AudioSpec get_AudioFormat() const;
	State get_state() const;
};

#endif SOUND_HPP