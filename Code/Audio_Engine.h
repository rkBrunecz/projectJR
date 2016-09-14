#ifndef Audio_Engine_H
#define Audio_Engine_H

#include "SFML\Audio.hpp"
#include "System.h"

class Audio_Engine
{
public:
	~Audio_Engine() { clearSoundList(); };

	static short addSound(std::string fileName, short volume);
	static void playSound(short id);
	static void clearSoundList();

private:
	struct Audio
	{
	public:
		Audio(std::string fileName, short volume)
		{
			this->fileName = fileName;

			System::load(&buff, fileName);
			sound.setBuffer(buff);
			sound.setVolume(volume);
		}

		void play()
		{
			if (sound.getStatus() == sf::Sound::Playing)
				sound.stop(); 

			sound.play();
		}

		bool isSameFile(std::string fileName)
		{
			if (this->fileName.compare(fileName) == 0)
				return true;

			return false;
		}

	private:
		sf::SoundBuffer buff;
		sf::Sound sound;
		std::string fileName;
	};

	static std::vector<Audio *> soundList;
};

#endif