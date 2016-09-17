#ifndef System_H
#define System_H

#include "SFML\Graphics.hpp"
#include "SFML\Audio.hpp"

class System
{
public:
	static void load(sf::Texture* tex, std::string fileName)
	{
		if (!tex->loadFromFile("res/Graphics/" + fileName))
		{
			printf("\"%s\" could not be found!", fileName);
			system("pause");
			exit(EXIT_FAILURE);
		}
	}

	static void load(sf::SoundBuffer* buffer, std::string fileName)
	{
		if (!buffer->loadFromFile("res/Sound/" + fileName))
		{
			printf("\"%s\" could not be found!", fileName);
			system("pause");
			exit(EXIT_FAILURE);
		}
	}
};

#endif