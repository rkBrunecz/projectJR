#ifndef Audio_Manager_H
#define Audio_Manager_H

#include "SFML\System\NonCopyable.hpp"
#include "Audio.h"

namespace pb
{
	class Audio_Manager : sf::NonCopyable
	{
	public:
		// Constructor
		Audio_Manager() {};

		// Destructor
		~Audio_Manager() { clearSoundList(); };

		// Public methods
		short addSound(Audio* a);
		void playSound(short id);
		void clearSoundList();

	private:
		std::vector<pb::Audio *> soundList;
	};
}

#endif