/*
Graphic_Engine.h
A namespace containing a list of textures used within the game. Contains methods for drawing all objects and freeing memory.

@author Randall Brunecz
@version 1.0 9/18/2016
*/

#ifndef Graphic_Manager_H
#define Graphic_Manager_H

#include <SFML\Graphics.hpp>
#include "PBE\System\System.h"
#include "PBE\System\Time.h"
#include "PBE\Graphic\Shader.h"
#include "PBE\Graphic\Day_Shift_Animation.h"

namespace pb
{
	class Graphic_Manager : sf::NonCopyable
	{
	public:
		// Constructors
		Graphic_Manager();
		Graphic_Manager(In_Game_Clock& clock);

		// Destructor
		~Graphic_Manager();

		const sf::Texture *addTexture(std::string fileName);
		void addToDrawList(sf::Sprite *s, bool isShadow);

		// Basic draw method
		void draw(sf::RenderWindow *window);

		// Use this draw method if you want to utilize the built in day and night shift, and shadow alpha shader
		void draw(sf::RenderWindow *window, Time& t);

		void clearTextureList();
		void toggleFrameRate();
		void dimScreen(sf::RenderWindow* window);
		void enableDayShift(bool enable);
		bool fadingIn(sf::RenderWindow *window);
		bool fadingOut(sf::RenderWindow *window);

	private:	
		struct Texture
		{
		public:
			void initialize(std::string fileName)
			{
				this->fileName = fileName;

				pb::load(&texture, fileName);
			}

			const sf::Texture* getTexture()
			{
				return &texture;
			}

			bool isSameFile(std::string fileName)
			{
				if (this->fileName.compare(fileName) == 0)
					return true;

				return false;
			}

		private:
			sf::Texture texture;
			std::string fileName;
		};

		struct Graphic_Obj
		{
			sf::Sprite *sprite;
			bool hasShadow;

			Graphic_Obj(sf::Sprite *s, bool hasShadow)
			{
				this->sprite = s;
				this->hasShadow = hasShadow;
			}
		};

		std::vector<Texture *> textures;
		std::vector<Graphic_Obj *> drawList;
		sf::Clock aniClock;
		sf::RectangleShape *fade;
		sf::RenderTexture *tex;
		pb::Shader *alpha;
		pb::Day_Shift_Animation *dayShift = NULL;
		bool updateTime = true;
	};
}


#endif