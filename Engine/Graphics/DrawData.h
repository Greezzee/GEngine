#pragma once
#include <list>
#include "SFML/Graphics.hpp"
#include "../Utility/Coordinate.h"
#include "ShaderManagment/Shader.h"
/*!
* Система "точек зрения" View отдалённо напоминает систему камер.
* 
* real_size - размер зоны в пикселях, на которой будут рисоваться спрайты, к которому применили данный View
* real_position - позиция верхнего левого угла зоны в пикселях. 
* Так, если real_position = (100, 200), то при рисовании спрайта с позицией (0, 0) с применением данного View, его реальная позиция
* окажется в (100, 200) пикселях относительно верхнего левого угла окна
* virtual_size есть просто положение нулевых координат в игровом простарнстве.
* 
* virtual_size - размер зоны, в которой находятся спрайты до применения к ним данного View.
* Так, если real_position = (0, 0), real_size = (1600, 900), а virtual_size = (1, 1), то если мы нарисуем спрайт
* с координатами (0.5, 0.5), то его изображение попадёт в точку (800, 450) на окне (в пикселях)
* 
* Например, с помощью этой системы, можно не заботиться о координатной системе игрового поля, ведь её можно задать не в пикселях,
* а в более удобных единицах. А при рисовании применять нужный View, чтобы переводить эти координаты в позицию на реальном экране
* 
* unit_vector задаёт направления осей координат. По умолчанию {1, 1} - оси координат по умолчанию в системе SFML.
* {1, -1} - стандартная Декартова система координат
* Чтобы развернуть ось x или ось y, замените 1 на -1
* Значения, отличные по модулю от 1, недопустимы
* 
*/


struct View
{
	Vector2F real_position;
	Vector2F real_size;
	Vector2F real_origin;
	Vector2F virtual_position;
	Vector2F virtual_size;
	Vector2F virtual_origin;
	Vector2F unit_vector = { 1, 1 };
};

struct Color
{
	Color() :
		r(255), g(255), b(255), a(255) {}
	Color(unsigned red, unsigned green, unsigned blue) :
		r(red), g(green), b(blue), a(255) {}
	Color(unsigned red, unsigned green, unsigned blue, unsigned alpha) :
		r(red), g(green), b(blue), a(alpha) {}
	unsigned r, g, b, a;

	static Color Red() { return { 255, 0, 0, 255 }; }
	static Color Green() { return { 0, 255, 0, 255 }; }
	static Color Blue() { return { 0, 0, 255, 255 }; }
	static Color White() { return { 255, 255, 255, 255 }; }
	static Color Black() { return { 0, 0, 0, 255 }; }
};


/*!
* Структура, содержащая информацию о всех преобразованиях спрайта, который нужно нарисовать на экране.
* Имеет конструкторы для более простой инициализации
*/
struct DrawData
{
	DrawData();
	DrawData(unsigned id, unsigned lr, Vector2F& pos);
	DrawData(unsigned id, unsigned lr, float x, float y);
	DrawData(unsigned id, unsigned lr, Vector2F& pos, float rot);
	DrawData(unsigned id, unsigned lr, float x, float y, float rot);
	DrawData(unsigned id, unsigned lr, Vector2F& pos, unsigned fr);
	DrawData(unsigned id, unsigned lr, float x, float y, unsigned fr);
	DrawData(unsigned id, unsigned lr, Vector2F& pos, float rot, unsigned fr);
	DrawData(unsigned id, unsigned lr, float x, float y, float rot, unsigned fr);

	unsigned spriteID; //! ID спрайта, используйте enum Sprites
	unsigned layer; //! Номер слоя. Чем больше значение, тем "выше" спрайт (перекроет всё что имеет меньший слой)

	Vector2F position; //! Позиция центра спрайта в пространстве
	Vector2F origin; //! Где находится центр спрайта относительно его левого верхнего угла. Измеряется от 0 до 1. {0, 0} - верхний левый угол, (0.5, 0.5) - центр спрайта
	Vector2F size; //! Размер спрайта в единицах длины игрового поля
	float rotation; //! Угол поворота спрайта В ГРАДУСАХ, против часовой стрелки
	int frame; //! Кадр анимации

	Color color; //! глобальный цвет спрайта ("прибавляется" к цветам спрайта), по умолчанию белый (никак не влияет на цвет спрайта)

	Shader* shader; //! шейдер, который использует данный спрайт
};

namespace tge
{

	struct Sprite
	{
		sf::Sprite sprite;
		Shader* shader;
	};

	struct GraphicLayer
	{
		std::vector<tge::Sprite> layer_sprites;
		Shader* layer_shader = nullptr;
		sf::RenderTexture* buffer = nullptr;
	};
}