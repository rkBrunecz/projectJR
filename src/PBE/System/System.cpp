#include "PBE\System\System.h"

namespace pb
{
	void System::load(sf::Texture *tex, std::string fileName)
	{
		if (!tex->loadFromFile("res/Graphics/" + fileName))
		{
			printf("\"%s\" could not be found!\n", fileName.c_str());
			system("pause");
			exit(EXIT_FAILURE);
		}
	}

	void System::load(sf::SoundBuffer *buffer, std::string fileName)
	{
		if (!buffer->loadFromFile("res/Sound/" + fileName))
		{
			printf("\"%s\" could not be found!\n", fileName.c_str());
			system("pause");
			exit(EXIT_FAILURE);
		}
	}

	void System::load(sf::Font *font, std::string fileName)
	{
		if (!font->loadFromFile("res/Font/" + fileName))
		{
			printf("\"%s\" could not be found!\n", fileName.c_str());
			system("pause");
			exit(EXIT_FAILURE);
		}
	}

	void System::load(sf::Shader *shader, std::string fileName, sf::Shader::Type type)
	{
		if (!shader->loadFromFile("res/Shaders/" + fileName, type))
		{
			printf("\"%s\" could not be found!\n", fileName.c_str());
			system("pause");
			exit(EXIT_FAILURE);
		}
	}

	void System::load(std::ifstream *stream, std::string fileName)
	{
		stream->open("res/Maps/" + fileName);

		if (stream->is_open())
		{
			printf("\"%s\" could not be found!\n", fileName.c_str());
			system("pause");
			exit(EXIT_FAILURE);
		}
	}
}