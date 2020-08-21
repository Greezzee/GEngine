#pragma once
#include "SFML/Graphics.hpp"
namespace tge {
	enum class ShaderType
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
			_type(ShaderType::EmptyShader) {}
		virtual ~Shader() {}

	protected:
		ShaderType _type;
		friend class ShaderManager;
	};
}