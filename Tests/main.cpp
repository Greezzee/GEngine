//#include <vld.h>
#include <iostream>
#include "../Engine/AllEngine.h"
class TestScene : public ge::Scene
{
public:
	void Init() override 
	{
		std::vector<ge::Vector2F> p = { {-70, -70}, {100, -100}, {60, -20}, {0, 100}, {-50, 150} };
		//a.Init(Vector2F(100, 100), p);
		//b.Init(Vector2F(400, 400), 150 * Vector2F(1, 1));
		//c.Init(Vector2F(680, 500), 75 * Vector2F(1, 1));
		a.Init(ge::Vector2F(100, 100), ge::Vector2F(50, 50));
		b.Init(ge::Vector2F(600, 600), ge::Vector2F(200, 200));
		ge::InputManager::LinkToCode(ge::KeyboardKey::W, 0);
		ge::InputManager::LinkToCode(ge::KeyboardKey::A, 1);
		ge::InputManager::LinkToCode(ge::KeyboardKey::S, 2);
		ge::InputManager::LinkToCode(ge::KeyboardKey::D, 3);
		ge::InputManager::LinkToCode(ge::KeyboardKey::Q, 4);
		ge::InputManager::LinkToCode(ge::KeyboardKey::E, 5);
		ge::InputManager::LinkToCode(ge::KeyboardKey::ESC, 6);

		ge::InputManager::LinkToCode(ge::KeyboardKey::MINUS, 7);
		ge::InputManager::LinkToCode(ge::KeyboardKey::EQUAL, 8);

		ge::View v;
		v.real_origin = { 0, 0 };
		v.real_position = { 0, 0 };
		v.real_size = { 1280, 720 };
		v.unit_vector = { 1, 1 };
		v.virtual_origin = { 0.5, 0.5 };
		v.virtual_size = { 1280, 720 };
		v.virtual_position = { 0, 0 };
		//GraphicManager::AddView(v);
		int text_id = ge::GraphicManager::LoadTexture("Tests/player.png");
		text = ge::GraphicManager::LoadSprite(ge::GraphicPrefabData(text_id, ge::Vector2F(32, 32), 3, ge::Vector2F(0, 32)));
		ge::GraphicManager::AddView({ {640, 360}, {1280, 720}, {0.5, 0.5}, {0, 0}, {1280, 720}, {0, 0}, {1, -1} });

		ge::GraphicManager::ClearSprites();
		//GraphicManager::ClearTextures();
		text = ge::GraphicManager::LoadSprite(ge::GraphicPrefabData(text_id, ge::Vector2F(32, 32), 2, ge::Vector2F(32, 0)));

		ge::LightManager::SetView(0);

		ge::LightManager::SetPixelSize(40);
		ge::LightManager::SetGlobalLight(ge::Color(100, 100, 100, 0));
		ge::GraphicManager::SetLayerShader(10, &x);
		ge::GraphicManager::SetLayerShader(12, &x);

		ge::GraphicManager::ShowFPS(true);

		printf("Shader: %d\n", sizeof(sf::Shader));
		printf("4x Vertex: %d\n", sizeof(sf::Vertex) * 4);
	}
	void Update() override 
	{
		if (ge::InputManager::IsPressed(7)) {
			ge::GraphicManager::SetResolution(ge::GraphicManager::GetResolution() + ge::Vector2U(16 * 5, 9 * 5));
		}
		if (ge::InputManager::IsPressed(8)) {
			ge::GraphicManager::SetResolution(ge::GraphicManager::GetResolution() - ge::Vector2U(16 * 5, 9 * 5));
		}
		ge::Vector2F speed = { 0, 0 };
		float rot = 0;
		if (ge::InputManager::IsDown(ge::KeyboardKey::S))
			speed.y += 1;
		if (ge::InputManager::IsDown(ge::KeyboardKey::W))
			speed.y -= 1;
		if (ge::InputManager::IsDown(ge::KeyboardKey::A))
			speed.x -= 1;
		if (ge::InputManager::IsDown(ge::KeyboardKey::D))
			speed.x += 1;
		if (ge::InputManager::IsDown(ge::KeyboardKey::Q))
			rot -= 1;
		if (ge::InputManager::IsDown(ge::KeyboardKey::E))
			rot += 1;

		speed = speed.Normalized();

		auto mouse = ge::GraphicManager::ConvertRealToView(ge::InputManager::GetMousePos(), 0);

		ge::LightManager::ClearLightSource();
		ge::LightData d;
		d.pos = mouse;
		d.color = {255, 255, 175, 0};
		d.full_dist = 40;
		d.any_dist = 240;
		d.softness = 1;
		ge::LightManager::AddLightSource(d);
		//auto mouse = InputManager::GetMousePos();
		//printf("Pos: %g : %g\n", mouse.x, mouse.y);

		a.SetPos(a.GetPos() + speed * 200 * ge::TimeManager::GetDeltaTimeF());
		a.SetAngle(a.GetAngle() + rot * ge::TimeManager::GetDeltaTimeF());
		ge::Debugger::DrawCollider(a, 15, 10, 0);
		ge::Debugger::DrawCollider(b, 15, 10, 0);
		//Debugger::DrawLine(a.GetPos() + Vector2F(0, a.GetRadius()) - Vector2F(1000, 0), a.GetPos() + Vector2F(0, a.GetRadius()) + Vector2F(1000, 0), 1, 1, Color::Green());
		//Debugger::DrawLine(a.GetPos() - Vector2F(0, a.GetRadius()) - Vector2F(1000, 0), a.GetPos() - Vector2F(0, a.GetRadius()) + Vector2F(1000, 0), 1, 1, Color::Green());


		float dist = ge::Collider::DistanceBetween(&a, &b, { 1, 0 });
		printf("%g\n", dist);
		if (ge::Collider::IsCollide((ge::UniversalCollider*)(&a), (ge::UniversalCollider*)(&b))) {
			ge::Debugger::DrawLine(a.GetPos(), b.GetPos(), 4, 0, ge::Color::Red());
		}
		for (int i = 0; i < 1280; i += 100)
			ge::Debugger::DrawLine({ (float)i, 0 }, { (float)i, 720 }, 2, 0);
		for (int i = 0; i < 720; i += 100)
			ge::Debugger::DrawLine({ 0, (float)i }, {1280, (float)i }, 2, 0);
		
		if (ge::InputManager::IsPressed(ge::KeyboardKey::ESC))
			ge::SceneManager::CloseScene(this);

		//Debugger::DrawLine({ 300, 720 - 360 }, { 640, 720 - 460 }, 10);
		//Debugger::DrawLine({ 640, 720 - 460 }, { 480, 720 - 120 }, 10);
		//Debugger::DrawLine({ 480, 720 - 120 }, { 300, 720 - 360 }, 10);

		
		ge::DrawData player;
		ge::Color colors[3] = { ge::Color::Red(), ge::Color::Blue(), ge::Color::Green() };
		player.layer = 12;
		player.origin = { 0.5, 0.5 };
		player.rotation = a.GetAngle() * 180 / ge::PI;
		player.size = { 100, 100 };
		player.spriteID = text;
		player.frame = (int)((float)ge::TimeManager::GetTimeFromLastRestart() / 1000000.f * 4);
		player.shader = nullptr;
		
		for (unsigned i = 0; i < 100; i+=19)
			for (unsigned j = 0; j < 100; j+=19)
			{
				player.frame++;
				player.color = colors[(i + j) % 3];
				player.position = { 12 * (float)i, 9 * (float)j };
				//GraphicManager::Draw(player, 1);
				//GraphicManager::Draw(player);
			}
		

		ge::Debugger::DrawLine({ 0, 0 }, { 1280, 0 }, 10, 1);
		ge::Debugger::DrawLine({ 0, 720 }, { 1280, 720 }, 10, 1);
		ge::Debugger::DrawLine({ 1280, 720 }, { 1280, 0 }, 10, 1);
		ge::Debugger::DrawLine({ 0, 0 }, { 0, 720 }, 10, 1);
	}
	void Destroy() override 
	{
		printf("Close!\n");
	}
private:
	ge::SquareCollider a;
	ge::SquareCollider b, c;
	ge::PixelLightShader x;
	int text;
};

class TestObj1 : public ge::GameObject
{
public:
	void Update() override
	{

	}

	void Destroy() override
	{

	}

	void Draw() override
	{

	}

	GameObject* Clone() const override
	{
		return new TestObj1;
	}
};
void test(ge::GameObject* t, void* data = nullptr) {
	if (t != nullptr) {
		ge::Vector2F a = t->GetPos();
		printf("Hi, I'm object at pos (%g; %g)!\n", a.x, a.y);
		std::cout << *((std::string*)data) << "\n";
	}
	else
		printf("But nobody came\n");
}
class GameplayScene : public ge::Scene
{
	void Init() override
	{
		field.Init(50, 2);
		ge::GameobjectSpawnData new_obj1, new_obj2, new_obj3;
		new_obj1.obj = new TestObj1;
		new_obj1.obj->SetPos(ge::Vector2F(1, 1));
		new_obj1.is_on_layer = { false, true };
		new_obj2.obj = new TestObj1;
		new_obj2.obj->SetPos(ge::Vector2F(2, 2));
		new_obj2.is_on_layer = { true, false };
		new_obj3.obj = new TestObj1;
		new_obj3.obj->SetPos(ge::Vector2F(3, 3));
		new_obj3.is_on_layer = { true, true };
		field.SpawnObject(new_obj1);
		field.SpawnObject(new_obj2);
		field.SpawnObject(new_obj3);
		field.ApplyToLayerObjects(0, test, &std::string("And I Say A"));
		field.KillObject(new_obj3.obj);
		field.ApplyToLayerObjects(0, test, &std::string("And I Say B"));
		field.ClearKilledObjects();
		field.ApplyToLayerObjects(0, test, &std::string("And I Say C"));
	}
	void Update() override
	{
		ge::SceneManager::CloseScene(this);
		//field.ApplyToAllObjects(test);
	}

	void Destroy() override
	{
		field.Destroy();
	}

	ge::ObjectContainer field;
private:
	
};

int main()
{
	ge::GameManager::Init();
	ge::Scene* start = new TestScene;
	start->Init();
	ge::GameManager::Launch(start);
	return 0;
}