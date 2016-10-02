#ifndef System_H
#define System_H

#include "SFML\Graphics.hpp"
#include "SFML\Audio.hpp"
#include <fstream>

namespace pb
{
	class System
	{
	public:
		static void load(sf::Texture *tex, std::string fileName);
		static void load(sf::SoundBuffer *buffer, std::string fileName);
		static void load(sf::Font *font, std::string fileName);
		static void load(sf::Shader *shader, std::string fileName, sf::Shader::Type type);
		static void load(std::ifstream *stream, std::string fileName);
	};
}

#endif