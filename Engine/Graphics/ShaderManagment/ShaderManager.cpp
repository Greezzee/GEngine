#include "ShaderManager.h"
#include "LightManager/LightManager.h"
#include <iostream>
using namespace tge;

std::vector<std::string> ShaderManager::_shaders_path = {
	"Engine/Graphics/Shaders/SmoothLight.frag",
	"Engine/Graphics/Shaders/PixelLight.frag",
	"Engine/Graphics/Shaders/Blur.frag"
};

std::vector<sf::Shader*> ShaderManager::_shaders;
bool ShaderManager::_is_aviable;

void ShaderManager::Init()
{
	if (!sf::Shader::isAvailable()) {
		printf("Shaders not available on this system\nShaders disabled\n");
		_is_aviable = false;
		return;
	}
	_shaders.resize(_shaders_path.size());
	_is_aviable = true;
	for (unsigned i = 0; i < _shaders.size(); i++) {
		_shaders[i] = new sf::Shader;
		bool x = _shaders[i]->loadFromFile(_shaders_path[i], sf::Shader::Fragment);
		if (!x) {
			_is_aviable = false;
			std::cout << "Shader: [" << _shaders_path[i] << "] not aviable\nShaders disabled\n";
		}
	}

}

void ShaderManager::Destroy()
{
	for (unsigned i = 0; i < _shaders.size(); i++)
		delete _shaders[i];
	_shaders.resize(0);
}

sf::Shader* ShaderManager::GetShader(Shader* shader)
{
	if (shader->_type == ShaderType::SmoothLightShader)
		LightManager::ApplyLight(_shaders[shader->_type]);
	if (shader->_type == ShaderType::PixelLightShader)
		LightManager::ApplyPixelLight(_shaders[shader->_type]);
	shader->ApplyParameters(_shaders[shader->_type]);
	return _shaders[shader->_type];
}

