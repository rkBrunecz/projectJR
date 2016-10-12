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
#include "PBE\Graphics\Shader.h"
#include "PBE\Graphics\Day_Shift_Animation.h"
#include "PBE\Graphics\Effect.h"

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
		void addToDrawList(sf::Drawable *d, bool isShadow);

		// Basic draw method
		void draw(sf::RenderWindow *window);

		// Use this draw method if you want to utilize the built in day and night shift, and shadow alpha shader
		void draw(sf::RenderWindow *window, Time& t);

		void clearTextureList();
		void enableDayShift(bool enable);

		// Non shader post effect methods
		void dimScreen(sf::Color dimColor, unsigned short alpha);
		void fadeIn(sf::Color fadeColor, float updateInterval, int increment);
		void fadeOut(sf::Color fadeColor, float updateInterval, int increment);
		void updateEffect(sf::RenderTarget& target);
		bool effectFinished();

	private:	
		struct Texture
		{
		public:
			void initialize(std::string fileName)
			{
				this->fileName = fileName;

				pb::System::load(&texture, fileName);
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
			sf::Drawable *drawable;
			bool hasShadow;

			Graphic_Obj(sf::Drawable *s, bool hasShadow)
			{
				this->drawable = s;
				this->hasShadow = hasShadow;
			}
		};

		std::vector<Texture *> textures;
		std::vector<Graphic_Obj *> drawList;
		sf::RenderTexture *buffer = 0; // Used to apply shaders
		pb::Shader *alpha = 0;
		pb::Effect *effect = 0; // Used for simple effects, like screen diming and fading
		pb::Day_Shift_Animation *dayShift = 0;
		bool updateTime = true;
	};
}


#endif