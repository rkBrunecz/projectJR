#ifndef Shader_H
#define Shader_H

#include "SFML\Graphics.hpp"

namespace pb
{
	class Shader
	{
	public:
		const sf::Shader *getShader()
		{
			return &shader;
		}

		void load()
		{
			if (!sf::Shader::isAvailable() || !loadShader())
				exit(EXIT_FAILURE);
		}

		void update(const short alpha)
		{
			updateParameters(alpha);
		}

	protected:
		Shader(const std::string name) : name(name) { };

		// PROTECTED VARIABLES
		std::string name;
		sf::Shader shader;

	private:
		// VIRTUAL METHODS
		virtual bool loadShader() = 0;

		// The various update methods
		virtual void updateParameters(const short alpha) { };
	};

	class Alpha : public Shader
	{
	public:
		Alpha(sf::Color color) : Shader("alpha")
		{
			pixelToAlpha = color;
		}

		void updateParameters(const short alpha)
		{
			pixelToAlpha.a = (sf::Uint8)alpha;

			shader.setParameter("texture", sf::Shader::CurrentTexture);
			shader.setParameter("color", pixelToAlpha);
		}

		bool loadShader()
		{
			if (!shader.loadFromFile("res/Shaders/" + name + ".frag", sf::Shader::Fragment))
			{
				printf("\"res/Shaders/%s.frag\" could not be found!\n", name.c_str());
				system("pause");

				return false;
			}

			return true;
		}

	private:
		sf::Color pixelToAlpha; // Color of pixel to alpha
	};
};

#endif