#include "BlurShader.h"

BlurShader::BlurShader()
{
	_type = tge::ShaderType::BlurShader;
}
BlurShader::~BlurShader()
{

}
void BlurShader::SetBlurRadius(float r)
{
	_radius = r;
}
float BlurShader::GetBlurRadius()
{
	return _radius;
}
void BlurShader::ApplyParameters(sf::Shader* shader)
{
	shader->setUniform("texture", sf::Shader::CurrentTexture);
	shader->setUniform("blur_radius", _radius);
}
