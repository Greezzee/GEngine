#include "GraphicManager.h"
#include "../Time/TimeManager.h"
#include "../AllEngine.h"
#include <cmath>
using namespace tge;
Vector2F GraphicManager::ConvertRealToView(Vector2F pos, unsigned view_id)
{
	View& view = views[view_id];
	pos -= view.real_position - view.real_size * view.real_origin;
	pos -= view.real_size * (Vector2F(1, 1) - view.unit_vector) / 2;
	pos = pos * view.unit_vector;
	pos = pos / view.real_size * view.virtual_size;

	pos += view.virtual_position;
	pos -= view.virtual_size * view.virtual_origin;

	return pos;
}

Vector2F GraphicManager::ConvertRealPosToView(Vector2F pos, unsigned view_id)
{
	View& view = views[view_id];
	pos -= view.real_position - view.real_size * view.real_origin;
	pos -= view.real_size * (Vector2F(1, 1) - view.unit_vector) / 2;
	pos = pos * view.unit_vector;
	pos = pos / view.real_size * view.virtual_size;

	pos += view.virtual_position;
	pos -= view.virtual_size * view.virtual_origin;

	return pos;
}

Vector2F GraphicManager::ConvertViewPosToReal(Vector2F pos, unsigned view_id)
{
	View& view = views[view_id];
	pos += view.virtual_size * view.virtual_origin;
	pos -= view.virtual_position;
	pos = pos * view.real_size / view.virtual_size;
	pos = pos / view.unit_vector;
	pos += view.real_size * (Vector2F(1, 1) - view.unit_vector) / 2;
	pos += view.real_position - view.real_size * view.real_origin;

	return pos;
}

float GraphicManager::ConvertViewSizeToReal(float len, unsigned view_id)
{
	View& view = views[view_id];
	len = len * view.real_size.x / view.virtual_size.x;
	return len;
}

sf::RenderWindow GraphicManager::window;
std::vector<GraphicPrefab> GraphicManager::sprites;
std::vector<std::list<tge::Sprite>> GraphicManager::to_draw;
unsigned GraphicManager::_sprites_count;
std::vector<int> GraphicManager::_basic_shapes;
unsigned GraphicManager::_engine_sprites_count;
tge::FPSCounter GraphicManager::_fps_counter;

std::vector<View> GraphicManager::views;
const unsigned GraphicManager::LAYER_COUNT = 20;

void GraphicManager::Init()
{
	window.create(sf::VideoMode(1280, 720), "Test", sf::Style::Titlebar | sf::Style::Close);
	//window.setFramerateLimit(65);

	_sprites_count = 0;
	sprites.resize(_engine_sprites_count);
	to_draw.resize(LAYER_COUNT);
	Sprite a;
	_engine_sprites_count = 2;
	_basic_shapes.resize(2);

	SetSpritesMaxCount(100);
	_basic_shapes[BasicShapes::Square] = LoadSprite(GraphicPrefabData("./Engine/Debugger/DebugResourses/Square.png", { 32, 32 }, 1));
	_basic_shapes[BasicShapes::Circle] = LoadSprite(GraphicPrefabData("./Engine/Debugger/DebugResourses/Circle.png", { 128, 128 }, 1));

	views.resize(1);
	views[0] = { {0, 0}, {1280, 720}, {0, 0}, {0, 0}, {1280, 720}, {0, 0}, {1, 1} };
	//views[Views::PLAYER_CAM] = { {0, 0}, {1280, 720}, {0, 0}, {0, 0}, {1600, 900}, {0.5, 0.5}, {1, -1}};
	//views[Views::MAIN_MENU] = { {0, 0}, {1280, 720}, {0, 0}, {0, 0}, {1600, 900}, {0, 0}, {1, -1}};

	tge::ShaderManager::Init();
}

bool GraphicManager::Update()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			return true;
	}

	window.clear();
	for (int i = 0; i < LAYER_COUNT; i++) {
		for (auto obj = to_draw[i].begin(); obj != to_draw[i].end(); obj++) {
			obj->shader ? window.draw((*obj).sprite, ShaderManager::GetShader((*obj).shader)) : window.draw((*obj).sprite);
		}
		to_draw[i].clear();
	}
	
	window.display();

	_fps_counter.Update();

	return false;
}

void GraphicManager::Exit()
{
	to_draw.resize(0);
	_basic_shapes.resize(0);
	sprites.resize(0);
	views.resize(0);
	window.close();
	tge::ShaderManager::Destroy();
}

bool GraphicManager::Draw(DrawData& data, unsigned view_id)
{
	if (data.spriteID >= _sprites_count || view_id >= views.size())
		return false;
	SetView(data, view_id);

	if (data.spriteID == -1)
		return false;

	GraphicPrefab& spr = sprites[data.spriteID];

	spr.sprite.setPosition(sf::Vector2f(data.position.x, data.position.y));
	spr.sprite.setRotation(data.rotation);
	spr.sprite.setColor(sf::Color(data.color.r, data.color.g, data.color.b, data.color.a));
	spr.sprite.setOrigin(sf::Vector2f(data.origin.x * spr.size.x, data.origin.y * spr.size.y));
	spr.sprite.setScale(sf::Vector2f(data.size.x / spr.size.x, data.size.y / spr.size.y));
	spr.sprite.setTextureRect(sf::IntRect(spr.size.x * (data.frame % spr.frames_count), 0, (int)spr.size.x, (int)spr.size.y));
	to_draw[data.layer].push_back({ sprites[data.spriteID].sprite, data.shader });
	return true;
}

void GraphicManager::SetView(DrawData& data, unsigned view_id)
{
	View& view = views[view_id];
	Vector2F obj_pos = data.position - view.virtual_position + view.virtual_size * view.virtual_origin;
	data.position = (obj_pos * view.real_size / view.virtual_size) * view.unit_vector;
	data.position -= view.real_size * (view.unit_vector - Vector2F(1, 1)) / 2.f;
	data.position += view.real_position - view.real_size * view.real_origin;
	data.size = data.size * view.real_size / view.virtual_size;
	data.rotation *= -1;
	
	auto view_pos = view.real_position - view.real_size * view.real_origin;

	float delta = fabsf(data.size.x) + fabsf(data.size.y);
	if (data.position.x + delta < view_pos.x ||
		data.position.y + delta < view_pos.y ||
		data.position.x - delta > view_pos.x + view.real_size.x ||
		data.position.y - delta > view_pos.y + view.real_size.y)
		data.spriteID = -1;
	
}

View* GraphicManager::GetView(unsigned view_id)
{
	if (view_id >= views.size())
		return nullptr;
	return &(views[view_id]);
}

void GraphicManager::ClearSprites()
{
	_sprites_count = 0;
}

unsigned GraphicManager::GetSpritesCount()
{
	return _sprites_count;
}

unsigned GraphicManager::GetSpritesMaxCount()
{
	return sprites.size();
}

bool GraphicManager::SetSpritesMaxCount(unsigned count)
{
	if (count < _engine_sprites_count)
		return false;
	sprites.resize(count);
	if (_sprites_count > count)
		_sprites_count = count;
	return true;
}

int GraphicManager::LoadSprite(GraphicPrefabData data)
{
	if (_sprites_count >= GetSpritesMaxCount())
		return -1;
	bool text_success = sprites[_sprites_count].texture.loadFromFile(data.file);
	if (!text_success)
		return -1;
	sprites[_sprites_count].sprite.setTexture(sprites[_sprites_count].texture);
	sprites[_sprites_count].size = data.size;
	sprites[_sprites_count].sprite.setTextureRect(sf::IntRect(0, 0, (int)data.size.x, (int)data.size.y));
	sprites[_sprites_count].frames_count = data.frames_count;
	_sprites_count++;
	return _sprites_count - 1;
}

bool GraphicManager::LoadSprite(GraphicPrefabData& data, unsigned id)
{
	if (id >= GetSpritesMaxCount() || id < _engine_sprites_count)
		return false;
	bool text_success = sprites[id].texture.loadFromFile(data.file);
	if (!text_success)
		return false;
	sprites[id].sprite.setTexture(sprites[id].texture);
	sprites[id].size = data.size;
	sprites[_sprites_count].sprite.setTextureRect(sf::IntRect(0, 0, (int)data.size.x, (int)data.size.y));
	sprites[_sprites_count].frames_count = data.frames_count;
	return true;
}

sf::RenderWindow* GraphicManager::GetWindow()
{
	return &window;
}

int GraphicManager::AddView(View view)
{
	if (fabsf(view.unit_vector.x) != 1 || fabsf(view.unit_vector.y) != 1)
		return -1;
	views.push_back(view);
	return views.size() - 1;
}

void GraphicManager::ResetViews()
{
	views.resize(1);
}

void GraphicManager::SetResolution(Vector2U new_size)
{
	window.create(sf::VideoMode(new_size.x, new_size.y), "Test", sf::Style::Titlebar | sf::Style::Close);
	views[0].real_size = Vector2F((float)new_size.x, (float)new_size.y);
	views[0].virtual_size = views[0].real_size;
}

Vector2U GraphicManager::GetResolution()
{
	auto size = window.getSize();
	return { size.x, size.y };
}

void GraphicManager::ShowFPS(bool is_active)
{
	_fps_counter.SetActive(is_active);
}
