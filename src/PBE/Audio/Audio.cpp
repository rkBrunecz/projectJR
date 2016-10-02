#include "PBE\Audio\Audio.h"
#include "PBE\System\System.h"

namespace pb
{
	Audio::Audio(std::string fileName, short volume)
	{
		this->fileName = fileName;

		pb::System::load(&buff, fileName);
		sound.setBuffer(buff);
		sound.setVolume(volume);
	}

	void Audio::play()
	{
		if (sound.getStatus() == sf::Sound::Playing)
			sound.stop();

		sound.play();
	}
}