#pragma once
#include "SFML/Graphics.hpp"
namespace tge {
	enum ShaderType
	{
		EmptyShader = -1,
		SmoothLightShader,
		PixelLightShader,
		BlurShader
	};
	class Shader
	{
	public:
		Shader() :
			_type(EmptyShader) {}
		virtual ~Shader() {}
	protected:
		ShaderType _type;
		virtual void ApplyParameters(sf::Shader* shader) = 0;

		friend class ShaderManager;
	};
}