#include "Audio_Engine.h"

std::vector<Audio_Engine::Audio *> Audio_Engine::soundList;

short Audio_Engine::addSound(std::string fileName, short volume)
{
	if (!soundList.empty())
	{
		for (unsigned int i = 0; i < soundList.size(); i++)
		{
			if (soundList[i]->isSameFile(fileName))
				return i;
		}

	}

	
	soundList.push_back(new Audio(fileName, volume));

	return soundList.size() - 1;
}

void Audio_Engine::playSound(short id)
{
	soundList[id]->play();
}

void Audio_Engine::clearSoundList()
{
	for (unsigned int i = 0; i < soundList.size(); i++)
		delete soundList[i];

	soundList.clear();
}