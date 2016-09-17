#ifndef Audio_H
#define Audio_H

#include <SFML\Audio.hpp>
#include "System.h"

class Audio
{
public:
	Audio(std::string fileName, short volume);
	
	friend bool operator== (const Audio &a1, const Audio &a2);

	void play();

private:
	sf::SoundBuffer buff;
	sf::Sound sound;
	std::string fileName;
};

#endif