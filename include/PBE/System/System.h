#ifndef System_H
#define System_H

#include "SFML\Graphics.hpp"
#include "SFML\Audio.hpp"
#include <fstream>

namespace pb
{
	void load(sf::Texture* tex, std::string fileName);
	void load(sf::SoundBuffer* buffer, std::string fileName);
	void load(std::ifstream* stream, std::string fileName);
}

#endif