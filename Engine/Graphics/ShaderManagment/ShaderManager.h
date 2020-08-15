#pragma once
#include <vector>
#include "Shader.h"
#include "SFML/Graphics.hpp"
namespace tge {
	class ShaderManager
	{
	public:
		static void Init();
		static sf::Shader* GetShader(Shader* shader);
		static void Destroy();
	private:
		static std::vector<std::string> _shaders_path;
		static std::vector<sf::Shader*> _shaders;
		static bool _is_aviable;
	};
}