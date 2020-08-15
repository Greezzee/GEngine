#pragma once
#include "Shader.h"


//! Задаёт шейдер пиксельного освещения. 
//! Источники света контролирует LightManager
class PixelLightShader : public tge::Shader
{
public:
	PixelLightShader();
	~PixelLightShader();

private:
	void ApplyParameters(sf::Shader* shader) override;
};
