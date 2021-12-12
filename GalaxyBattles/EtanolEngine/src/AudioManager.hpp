#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <Sound.hpp>

class AudioManager
{
public:
	std::shared_ptr<Sound> createSound(std::string_view file_name, bool is_loop, int volume);
	void update();
private:
	static void audio_callback(void* userdata, uint8_t* stream, int len);
	SDL_AudioDeviceID _audio_device;
	bool pause_status = true;
	SDL_AudioSpec _audio_spec_from_file{};
	std::vector<std::weak_ptr<Sound>> _buffers;
};