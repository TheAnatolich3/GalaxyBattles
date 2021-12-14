#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <Sound.hpp>

class AudioManager
{
public:
	AudioManager();
	std::shared_ptr<Sound> createSound(std::string_view file_name, bool is_loop, int volume) const;
	void update();
private:
	static void audio_callback(void* userdata, uint8_t* stream, int len);
	SDL_AudioDeviceID _audio_device;
	bool pause_status = true;
	SDL_AudioSpec _wanted_spec{};
	mutable std::vector<std::weak_ptr<Sound>> _buffers;
};