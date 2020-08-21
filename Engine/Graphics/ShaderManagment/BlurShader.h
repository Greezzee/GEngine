#pragma once
#include "Shader.h"
#include "ShaderManager.h"
//! Задаёт шейдер "размытия"
class BlurShader : public tge::Shader
{
public:
	BlurShader();
	~BlurShader();
	//! Устнавливает уровень размытия. Малое число, порядка 0.01
	static void SetBlurRadius(float r);
	static float GetBlurRadius();

private:
	static float _radius;
	static sf::Shader my_shader;
	friend class tge::ShaderManager;
};
