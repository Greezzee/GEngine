#include <vld.h>

#include "../Engine/AllEngine.h"
class TestScene : public Scene
{
public:
	void Init() override 
	{
		std::vector<Vector2F> p = { {-70, -70}, {100, -100}, {60, -20}, {0, 100}, {-50, 150} };
		//a.Init(Vector2F(100, 100), p);
		//b.Init(Vector2F(400, 400), 150 * Vector2F(1, 1));
		//c.Init(Vector2F(680, 500), 75 * Vector2F(1, 1));
		a.Init(Vector2F(100, 100), 0, Vector2F(100, 50));
		b.Init(Vector2F(400, 400), PI / 4, Vector2F(200, 50));
		InputManager::LinkToCode(KeyboardKey::W, 0);
		InputManager::LinkToCode(KeyboardKey::A, 1);
		InputManager::LinkToCode(KeyboardKey::S, 2);
		InputManager::LinkToCode(KeyboardKey::D, 3);
		InputManager::LinkToCode(KeyboardKey::Q, 4);
		InputManager::LinkToCode(KeyboardKey::E, 5);
		InputManager::LinkToCode(KeyboardKey::ESC, 6);

		InputManager::LinkToCode(KeyboardKey::MINUS, 7);
		InputManager::LinkToCode(KeyboardKey::EQUAL, 8);

		View v;
		v.real_origin = { 0, 0 };
		v.real_position = { 0, 0 };
		v.real_size = { 1280, 720 };
		v.unit_vector = { 1, 1 };
		v.virtual_origin = { 0.5, 0.5 };
		v.virtual_size = { 1280, 720 };
		v.virtual_position = { 0, 0 };
		//GraphicManager::AddView(v);
		text = GraphicManager::LoadSprite(GraphicPrefabData("Tests/player.png", Vector2F(96, 96), 1));
		//GraphicManager::AddView({ {680, 360}, {1280, 720}, {0.5, 0.5}, {100, 100}, {1280, 720}, {0.5, 0.5}, {1, -1} });

		LightManager::SetView(0);

		LightManager::SetPixelSize(40);
		LightManager::SetGlobalLight(Color(50, 50, 50, 0));
		GraphicManager::SetLayerShader(10, &x);
		GraphicManager::SetLayerShader(12, &x);
	}
	void Update() override 
	{
		if (InputManager::IsPressed(7)) {
			GraphicManager::SetResolution(GraphicManager::GetResolution() + Vector2U(16 * 5, 9 * 5));
		}
		if (InputManager::IsPressed(8)) {
			GraphicManager::SetResolution(GraphicManager::GetResolution() - Vector2U(16 * 5, 9 * 5));
		}

		Vector2F speed = { 0, 0 };
		float rot = 0;
		if (InputManager::IsDown(KeyboardKey::S))
			speed.y += 1;
		if (InputManager::IsDown(KeyboardKey::W))
			speed.y -= 1;
		if (InputManager::IsDown(KeyboardKey::A))
			speed.x -= 1;
		if (InputManager::IsDown(KeyboardKey::D))
			speed.x += 1;
		if (InputManager::IsDown(KeyboardKey::Q))
			rot -= 1;
		if (InputManager::IsDown(KeyboardKey::E))
			rot += 1;

		speed = speed.Normalized();

		auto mouse = GraphicManager::ConvertRealToView(InputManager::GetMousePos(), 0);

		LightManager::ClearLightSource();
		LightData d;
		d.pos = mouse;
		d.color = {255, 255, 175, 0};
		d.full_dist = 40;
		d.any_dist = 240;
		d.softness = 1;
		LightManager::AddLightSource(d);
		//auto mouse = InputManager::GetMousePos();
		//printf("Pos: %g : %g\n", mouse.x, mouse.y);

		a.SetPos(a.GetPos() + speed * 200 * TimeManager::GetDeltaTimeF());
		a.SetAngle(a.GetAngle() + rot * TimeManager::GetDeltaTimeF());
		Debugger::DrawCollider(a);
		Debugger::DrawCollider(b);
		//Debugger::DrawLine(a.GetPos() + Vector2F(0, a.GetRadius()) - Vector2F(1000, 0), a.GetPos() + Vector2F(0, a.GetRadius()) + Vector2F(1000, 0), 1, 1, Color::Green());
		//Debugger::DrawLine(a.GetPos() - Vector2F(0, a.GetRadius()) - Vector2F(1000, 0), a.GetPos() - Vector2F(0, a.GetRadius()) + Vector2F(1000, 0), 1, 1, Color::Green());


		//float dist = Collider::DistanceBetween(&a, &PolygonCollider(b), { 1, 0 });
		//printf("%g\n", dist);
		if (Collider::IsCollide((UniversalCollider*)(&a), (UniversalCollider*)(&b))) {
			Debugger::DrawLine(a.GetPos(), b.GetPos(), 4, 0, Color::Red());
		}
		for (int i = 0; i < 1280; i += 100)
			Debugger::DrawLine({ (float)i, 0 }, { (float)i, 720 }, 2);
		for (int i = 0; i < 720; i += 100)
			Debugger::DrawLine({ 0, (float)i }, {1280, (float)i }, 2);
		
		if (InputManager::IsPressed(KeyboardKey::ESC))
			SceneManager::CloseScene(this);

		//Debugger::DrawLine({ 300, 720 - 360 }, { 640, 720 - 460 }, 10);
		//Debugger::DrawLine({ 640, 720 - 460 }, { 480, 720 - 120 }, 10);
		//Debugger::DrawLine({ 480, 720 - 120 }, { 300, 720 - 360 }, 10);

		/*
		DrawData player;
		player.frame = 0;
		player.color = Color::White();
		player.layer = 12;
		player.origin = { 0, 0 };
		player.rotation = 0;
		player.size = { 200, 200 };
		player.spriteID = text;
		player.shader = nullptr;
		for (unsigned i = 0; i < 100; i++)
			for (unsigned j = 0; j < 100; j++)
			{
				player.position = { 12 * (float)i, 9 * (float)j };
				GraphicManager::Draw(player);
			}
		*/

		Debugger::DrawLine({ 0, 0 }, { 1280, 0 }, 10);
		Debugger::DrawLine({ 0, 720 }, { 1280, 720 }, 10);
		Debugger::DrawLine({ 1280, 720 }, { 1280, 0 }, 10);
		Debugger::DrawLine({ 0, 0 }, { 0, 720 }, 10);
	}
	void Destroy() override 
	{
		printf("Close!\n");
	}
private:
	EllipseCollider a;
	EllipseCollider b, c;
	PixelLightShader x;
	int text;
};

class TestObj1 : public GameObject
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
void test(GameObject* t) {
	if (t != nullptr) {
		Vector2F a = t->GetPos();
		printf("Hi, I'm object at pos (%g; %g)!\n", a.x, a.y);
	}
	else
		printf("But nobody came\n");
}
class GameplayScene : public Scene
{
	void Init() override
	{
		field.Init(50, 2);
		GameobjectSpawnData new_obj1, new_obj2, new_obj3;
		new_obj1.obj = new TestObj1;
		new_obj1.obj->SetPos(Vector2F(1, 1));
		new_obj1.is_on_layer = { false, true };
		new_obj2.obj = new TestObj1;
		new_obj2.obj->SetPos(Vector2F(2, 2));
		new_obj2.is_on_layer = { true, false };
		new_obj3.obj = new TestObj1;
		new_obj3.obj->SetPos(Vector2F(3, 3));
		new_obj3.is_on_layer = { true, true };
		field.SpawnObject(new_obj1);
		field.SpawnObject(new_obj2);
		field.SpawnObject(new_obj3);
		field.ApplyToLayerObjects(test, 5);
		field.KillObject(new_obj3.obj);
		field.ApplyToLayerObjects(test, 15);
		field.ClearKilledObjects();
		field.ApplyToLayerObjects(test, 48);
	}
	void Update() override
	{
		SceneManager::CloseScene(this);
		//field.ApplyToAllObjects(test);
	}

	void Destroy() override
	{
		field.Destroy();
	}

	ObjectContainer field;
private:
	
};

int main()
{
	GameManager::Init();
	Scene* start = new GameplayScene;
	start->Init();
	GameManager::Launch(start);
	return 0;
}