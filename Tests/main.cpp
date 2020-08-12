//#include <vld.h>

#include "../Engine/AllEngine.h"

class TestScene : public Scene
{
public:
	void Init() override 
	{
		std::vector<Vector2F> p = { {-70, -70}, {100, -100}, {60, -20}, {0, 100}, {-50, 150} };
		a.Init(Vector2F(100, 100), p);
		b.Init(Vector2F(400, 400), 150 * Vector2F(1, 1));
		c.Init(Vector2F(680, 500), 75 * Vector2F(1, 1));
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
	}
	void Update() override 
	{
		if (InputManager::IsPressed(7)) {
			GraphicManager::ShowFPS(true);
		}
		if (InputManager::IsPressed(8)) {
			GraphicManager::ShowFPS(false);
		}

		Vector2F speed = { 0, 0 };
		float rot = 0;
		if (InputManager::IsDown(2))
			speed.y += 1;
		if (InputManager::IsDown(0))
			speed.y -= 1;
		if (InputManager::IsDown(1))
			speed.x -= 1;
		if (InputManager::IsDown(3))
			speed.x += 1;
		if (InputManager::IsDown(4))
			rot -= 1;
		if (InputManager::IsDown(5))
			rot += 1;

		speed = speed.Normalized();

		auto mouse = GraphicManager::ConvertRealToView(InputManager::GetMousePos(), 0);
		//auto mouse = InputManager::GetMousePos();
		//printf("Pos: %g : %g\n", mouse.x, mouse.y);

		a.SetPos(a.GetPos() + speed * 200 * TimeManager::GetDeltaTimeF());
		a.SetAngle(a.GetAngle() + rot * TimeManager::GetDeltaTimeF());
		Debugger::DrawCollider(a, 10, 4);
		Debugger::DrawCollider(b, 10, 4);
		//Debugger::DrawLine(a.GetPos() + Vector2F(0, a.GetRadius()) - Vector2F(1000, 0), a.GetPos() + Vector2F(0, a.GetRadius()) + Vector2F(1000, 0), 1, 1, Color::Green());
		//Debugger::DrawLine(a.GetPos() - Vector2F(0, a.GetRadius()) - Vector2F(1000, 0), a.GetPos() - Vector2F(0, a.GetRadius()) + Vector2F(1000, 0), 1, 1, Color::Green());


		float dist = Collider::DistanceBetween(&a, &PolygonCollider(b), { 1, 0 });
		//printf("%g\n", dist);
		if (fabsf(dist) < 10) {
			Debugger::DrawLine(a.GetPos(), b.GetPos(), 4, 0, Color::Red());
		}
		for (int i = 0; i < 1280; i += 100)
			Debugger::DrawLine({ (float)i, 0 }, { (float)i, 720 }, 2);
		for (int i = 0; i < 720; i += 100)
			Debugger::DrawLine({ 0, (float)i }, {1280, (float)i }, 2);
		
		if (InputManager::IsPressed(6))
			SceneManager::CloseScene(this);
	}
	void Destroy() override 
	{
		printf("Close!\n");
	}
private:
	PolygonCollider a;
	SquareCollider b, c;
};

class TestObj : public GameObject
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
		return new TestObj;
	}
};

class GameplayScene : public Scene
{
	void Init() override
	{
		field.Init();
		GameobjectSpawnData new_obj;
		new_obj.obj = new TestObj;
		field.SpawnObject(new_obj);
	}
	void Update() override
	{
		field.Update();
	}

	void Destroy() override
	{
		field.Destroy();
	}

	GameField field;
};

int main()
{
	GameManager::Init();
	Scene* start = new TestScene;
	start->Init();
	GameManager::Launch(start);
	return 0;
}