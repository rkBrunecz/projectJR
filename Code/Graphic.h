/*
Graphic.h
Parent class that defines common methods that child objects will share.

@author Randall Brunecz
@version 1.0 9/1/2015
*/

#ifndef Graphic_H
#define Graphic_H

#include <SFML\Graphics.hpp>
#include "System.h"

class Graphic
{
public:
	// PUBLIC FUNCTIONS
	static sf::Texture * addTexture(std::string fileName);
	static void initializeData();
	static void freeMemory();
	static void addToDrawList(sf::Sprite *s, bool isShadow);
	static void draw(sf::RenderWindow *window);
	static void clearTextureList();
	static void toggleFrameRate();
	static void dimScreen(sf::RenderWindow* window);
	static void enableDayShift(bool enable) { updateTime = enable; };
	static bool fadingIn(sf::RenderWindow *window);
	static bool fadingOut(sf::RenderWindow *window);


	// OVERRIDABLE PUBLIC FUNCTIONS
	virtual ~Graphic() { };
	virtual void updateDrawList(bool animate);
	virtual void updatePosition();
	virtual void setColor(int r, int g, int b, int a);
	virtual void initialize() { };
	virtual sf::IntRect getRect();

	virtual bool collisionDetected(sf::IntRect* tmp);

private:
	// CONSTANTS
	static const sf::Color MORNING, DAWN, AFTERNOON, EVENING;
	static const short SHADOW_ALPHA_MAX = 100;

	// PRIVATE STATIC METHODS
	static void updateDayTime();
	
	static enum TimeOfDay 
	{
		Morning,
		Dawn,
		Afternoon,
		Evening
	} shiftTo;

	struct Texture 
	{
	public:
		void initialize(std::string fileName)
		{
			this->fileName = fileName;

			System::load(&texture, fileName);
		}
		
		sf::Texture* getTexture()
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
		bool isShadow;

		Graphic_Obj(sf::Sprite *s, bool isShadow)
		{
			this->sprite = s;
			this->isShadow = isShadow;
		}
	};

	static std::vector<Texture *> textures;
	static std::vector<Graphic_Obj *> drawList;
	static sf::Clock clock;
	static sf::Color dayShift, colorIncrement;
	static sf::RectangleShape *fade;
	static float dayUpdateInterval;
	static short shadowAlpha;
	static bool updateTime;
	static sf::Shader* alpha;
	static sf::RenderTexture *tex;
};

#endif