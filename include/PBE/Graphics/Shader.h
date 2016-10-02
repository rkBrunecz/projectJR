#ifndef Shader_H
#define Shader_H

#include "SFML\Graphics.hpp"
#include "PBE\System\System.h"

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
			if (!sf::Shader::isAvailable())
				exit(EXIT_FAILURE);

			loadShader();
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
		virtual void loadShader() = 0;

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

			shader.setUniform("texture", sf::Shader::CurrentTexture);
			shader.setUniform("color", sf::Glsl::Vec4(pixelToAlpha));
		}

		void loadShader()
		{
			pb::System::load(&shader, name + ".frag", sf::Shader::Fragment);
		}

	private:
		sf::Color pixelToAlpha; // Color of pixel to alpha
	};
};

#endif