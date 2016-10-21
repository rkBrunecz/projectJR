#ifndef Shader_H
#define Shader_H

#include "SFML\Graphics.hpp"
#include "PBE\System\System.h"
#include "PBE\Graphics\Light.h"

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

		void update(const sf::Vector3f *lightPositions, const sf::Glsl::Vec4 *colors, const float *intensities, int numLights)
		{
			updateParameters(lightPositions, colors, intensities, numLights);
		}

		void update(const short alpha, float maxOpacity, const sf::Vector3f *lightPositions, int numLights)
		{
			updateParameters(alpha, maxOpacity, lightPositions, numLights);
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
		virtual void updateParameters(const sf::Vector3f *lightPositions, const sf::Glsl::Vec4 *colors, const float *intensities, int numLights) { };
		virtual void updateParameters(const short alpha, float maxOpacity, const sf::Vector3f *lightPositions, int numLights) { };
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

	class Lighting : public Shader
	{
	public:
		Lighting() : Shader("Lighting")
		{
		
		}

		void updateParameters(const sf::Vector3f *lightPositions, const sf::Glsl::Vec4 *colors, const float *intensities, int numLights)
		{
			shader.setUniform("texture", sf::Shader::CurrentTexture);
			
			if (numLights != 0)
			{
				shader.setUniformArray("lightColor", colors, numLights);
				shader.setUniformArray("lights", lightPositions, numLights);
				shader.setUniformArray("intensities", intensities, numLights);
			}

			shader.setUniform("numLights", numLights);
		}

		void loadShader()
		{
			pb::System::load(&shader, "lighting.frag", sf::Shader::Fragment);
		}
	};

	class Shadow : public Shader
	{
	public:
		Shadow() : Shader("Shadow")
		{

		}

		void updateParameters(const short alpha, float maxOpacity, const sf::Vector3f *lightPositions, int numLights)
		{
			sf::Color c = sf::Color::Black;
			c.a = (sf::Uint8)alpha;

			maxOpacity = maxOpacity / 255;

			shader.setUniform("texture", sf::Shader::CurrentTexture);
			shader.setUniform("color", sf::Glsl::Vec4(c));

			if (numLights != 0)
				shader.setUniformArray("lights", lightPositions, numLights);


			shader.setUniform("maxOpacity", maxOpacity);
			shader.setUniform("numLights", numLights);
		}

		void loadShader()
		{
			pb::System::load(&shader, "shadow.frag", sf::Shader::Fragment);
		}
	};
};

#endif