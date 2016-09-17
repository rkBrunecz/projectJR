#include "Audio.h"

Audio::Audio(std::string fileName, short volume)
{
	this->fileName = fileName;

	System::load(&buff, fileName);
	sound.setBuffer(buff);
	sound.setVolume(volume);
}

bool operator== (const Audio &a1, const Audio &a2)
{
	if (a1.fileName.compare(a2.fileName) == 0)
		return true;

	return false;
}

void Audio::play()
{
	if (sound.getStatus() == sf::Sound::Playing)
		sound.stop();

	sound.play();
}