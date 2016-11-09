#ifndef Light_H
#define Light_H

#include "SFML\Graphics\View.hpp"
#include "SFML\Graphics\Color.hpp"
#include "SFML\System\Vector2.hpp"
#include "SFML\System\Vector3.hpp"

namespace pb
{
	class Light
	{
	public:
		sf::Color lightColor;
		sf::Vector2f lightPos;
		float steps;
		float intensity;

		Light(const sf::Color& lightColor, const sf::Vector2f& lightPos, float steps, float intensity)
		{
			this->lightColor = lightColor;
			this->lightPos = lightPos;
			this->steps = steps;
			this->intensity = intensity;
		}

		Light(const sf::Color& lightColor, float steps, float intensity)
		{
			this->lightColor = lightColor;
			this->steps = steps;
			this->intensity = intensity;
		}

		sf::Vector3f convertToScreenCoordinates(const sf::View& v, const sf::Vector2u& windowSize)
		{
			sf::Vector3f screenPosition;
			float scale = (windowSize.x / v.getSize().x);;

			screenPosition.x = (lightPos.x - (v.getCenter().x - (v.getSize().x / 2))) * scale;
			screenPosition.y = windowSize.y - ((lightPos.y - (v.getCenter().y - (v.getSize().y / 2))) * scale);
			screenPosition.z = steps * scale;

			return screenPosition;
		}
	};
}

#endif
