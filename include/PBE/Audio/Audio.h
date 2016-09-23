#ifndef Audio_H
#define Audio_H

#include <SFML\Audio.hpp>

namespace pb
{
	class Audio
	{
	public:
		Audio(std::string fileName, short volume);

		friend bool operator== (const Audio &a1, const Audio &a2)
		{
			if (a1.fileName.compare(a2.fileName) == 0)
				return true;

			return false;
		}

		void play();

	private:
		sf::SoundBuffer buff;
		sf::Sound sound;
		std::string fileName;
	};
}

#endif