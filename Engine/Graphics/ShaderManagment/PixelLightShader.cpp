#include "PixelLightShader.h"

PixelLightShader::PixelLightShader()
{
	_type = tge::ShaderType::PixelLightShader;
}
PixelLightShader::~PixelLightShader()
{

}

void PixelLightShader::ApplyParameters(sf::Shader* shader)
{
	shader->setUniform("texture", sf::Shader::CurrentTexture);
	shader->setUniform("global_color", sf::Glsl::Vec4(1.f, 1.f, 1.f, 1.f));
}
