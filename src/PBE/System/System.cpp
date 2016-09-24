#include "PBE\System\System.h"

namespace pb
{
	void load(sf::Texture* tex, std::string fileName)
	{
		if (!tex->loadFromFile("res/Graphics/" + fileName))
		{
			printf("\"%s\" could not be found!", fileName);
			system("pause");
			exit(EXIT_FAILURE);
		}
	}

	void load(sf::SoundBuffer* buffer, std::string fileName)
	{
		if (!buffer->loadFromFile("res/Sound/" + fileName))
		{
			printf("\"%s\" could not be found!", fileName);
			system("pause");
			exit(EXIT_FAILURE);
		}
	}

	void load(std::ifstream* stream, std::string fileName)
	{
		stream->open("res/Maps/" + fileName);

		if (stream->is_open())
		{
			printf("\"%s\" could not be found!", fileName);
			system("pause");
			exit(EXIT_FAILURE);
		}
	}
}