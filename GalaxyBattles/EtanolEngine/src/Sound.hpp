#include <string_view>
#include <SDL.h>

class Sound
{
public:
	Sound(std::string_view filename, bool is_stream, bool is_loop);

	void play();
	void pause();
private:
	SDL_AudioDeviceID _audio_device;
	bool _isStream = false;
	bool _isLoop = false;
};