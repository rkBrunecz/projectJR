#include "PBE\Audio\Audio_Manager.h"

namespace pb
{

	short Audio_Manager::addSound(Audio* a)
	{
		if (!soundList.empty())
		{
			for (unsigned int i = 0; i < soundList.size(); i++)
			{
				if (*soundList[i] == *a)
				{
					delete a; // Delete a if it already exists in the soundlist
					return i;
				}
			}

		}

		soundList.push_back(a);

		return soundList.size() - 1;
	}

	void Audio_Manager::playSound(short id)
	{
		soundList[id]->play();
	}

	void Audio_Manager::clearSoundList()
	{
		for (unsigned int i = 0; i < soundList.size(); i++)
			delete soundList[i];

		soundList.clear();
	}
}