#ifndef Audio_Engine_H
#define Audio_Engine_H

#include "SFML\Audio.hpp"
#include "Audio.h"

class Audio_Engine
{
public:
	~Audio_Engine() { clearSoundList(); };

	static short addSound(Audio* a);
	static void playSound(short id);
	static void clearSoundList();

private:
	static std::vector<Audio *> soundList;
};

#endif