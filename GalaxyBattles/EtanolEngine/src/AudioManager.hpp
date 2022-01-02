#ifndef AUDIO_MANAGER_HPP
#define AUDIO_MANAGER_HPP

#include <string_view>
#include <vector>
#include <memory>
#include <mutex>
#include <Sound.hpp>


class AudioManager
{
public:
	AudioManager();
	std::shared_ptr<Sound> createSound(std::string_view file_name, bool is_loop, float volume) const;
	void update();
	void set_volume(float volume) const;
	void change_pause_status()
	{
		_pause_status = !_pause_status;
	}
private:
	static void audio_callback(void* userdata, uint8_t* stream, int len);
	SDL_AudioDeviceID _audio_device;
	bool _pause_status = false;
	mutable float _volume = 1.0f;
	SDL_AudioSpec _wanted_spec{};
	mutable std::vector<std::weak_ptr<Sound>> _buffers;
	std::mutex _lock_buffer;
};
#endif AUDIO_MANAGER_HPP