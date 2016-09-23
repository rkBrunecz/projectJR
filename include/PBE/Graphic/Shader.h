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
			{
				system("pause");
				exit(EXIT_FAILURE);
			}
		}

		void update(const float alpha)
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
		virtual void updateParameters(const float alpha) { };
	};

	class Alpha : public Shader
	{
	public:
		Alpha() : Shader("alpha")
		{

		}

		void updateParameters(const float alpha)
		{
			shader.setParameter("texture", sf::Shader::CurrentTexture);
			shader.setParameter("alpha", alpha / RGB_VALUE_MAX);
		}

		bool loadShader()
		{
			return shader.loadFromFile("res/Shaders/" + name + ".frag", sf::Shader::Fragment);
		}

	private:
		const float RGB_VALUE_MAX = 255;
		sf::Color pixelToAlpha; // Color of pixel to alpha
	};
};

#endif