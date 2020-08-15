#pragma once
#include "Shader.h"

//! Шейдер гладкого освещения
//! Источники света контролирует LightManager
class SmoothLightShader : public tge::Shader
{
public:
	SmoothLightShader();
	~SmoothLightShader();

private:
	void ApplyParameters(sf::Shader* shader) override;
};
