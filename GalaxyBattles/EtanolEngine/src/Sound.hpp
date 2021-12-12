#include <string_view>
#include <SDL.h>

class Sound
{
public:
	Sound(std::string_view filename, bool is_loop);

	void play();
	void pause();
private:
	enum class State {
		ST_PLAY,
		ST_PAUSE
	};

	SDL_AudioDeviceID _audio_device;
	bool _isLoop = false;
};