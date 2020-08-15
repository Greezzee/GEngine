#pragma once
#include "Shader.h"

//! Задаёт шейдер "размытия"
class BlurShader : public tge::Shader
{
public:
	BlurShader();
	~BlurShader();
	//! Устнавливает уровень размытия. Малое число, порядка 0.01
	void SetBlurRadius(float r);
	float GetBlurRadius();

private:
	void ApplyParameters(sf::Shader* shader) override;
	float _radius;
};
