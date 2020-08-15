#include "SmoothLightShader.h"

SmoothLightShader::SmoothLightShader()
{
	_type = tge::ShaderType::SmoothLightShader;
}
SmoothLightShader::~SmoothLightShader()
{

}

void SmoothLightShader::ApplyParameters(sf::Shader* shader)
{
	shader->setUniform("texture", sf::Shader::CurrentTexture);
	shader->setUniform("global_color", sf::Glsl::Vec4(1.f, 1.f, 1.f, 1.f));
}
