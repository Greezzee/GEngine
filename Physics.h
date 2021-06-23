#include "../../Engine/AllEngine.h"
#include <iostream>
#include <vector>

class Circle : public GameObject
{
public:
	void Init() {
		mass_ = 4;
		collider_.SetRadius(mass_ * 5.f);
		_pos = { 300, 300 };
		speed_ = { 500, 0 };
		//std::cout << "Obj created\n";
	}

	void Init(Vector2F pos, Vector2F speed, float mass = 4, float bf = 1, int q = 0) {
		mass_ = mass;
		collider_.SetRadius(mass_ * 5.f);
		_pos = pos;
		speed_ = speed;
		//std::cout << "Obj created\n";
		q_ = q;
		bounce_factor = bf;
		switch ((int)(bf * 4))
		{
		case 4:
			color = Color::White();
			break;
		case 3:
			color = Color::Blue();
			break;
		case 2:
			color = Color::Red();
			break;
		case 0:
			color = Color::Green();
			break;
		}
	}

	//! Обновление объекта. Вызывается каждый кадр (на данный момент. В будущем появится пауза и данная функция вызываться в ней не будет)
	void Update() {
		collider_.SetPos(_pos);
		speed_.y += 200.f * TimeManager::GetDeltaTimeF();

		_pos += speed_ * TimeManager::GetDeltaTimeF();

		if (_pos.y < 0 || _pos.y > 720 - mass_ * 5.0) {
			if (_pos.y < 0)
				_pos.y = 0;
			if (_pos.y > 720 - mass_ * 5.0)
				_pos.y = 720 - mass_ * 5.0;
			speed_.y *= -bounce_factor;
		}
		if (_pos.x < 0 || _pos.x > 1280) {
			if (_pos.x < 0)
				_pos.x = 0;
			if (_pos.x > 1280)
				_pos.x = 1280;
			speed_.x *= -bounce_factor;
		}

	}

	//! Рисование объекта. Переопределяйте вручную! Вызывается вообще каждый кадр (даже на паузе)
	void Draw() {
		Debugger::DrawCollider(collider_, 0, color);
	}

	//! Уничтожение объекта. Вызывается 1 раз, когда игровое поле уничтожает объект
	void Destroy() {

	}

	void TestCollision(Circle& other) {
		//float dist = (pos_ - other.pos_).Magnitude();
		//if (dist >= 1) {
		//	Vector2F accel = q_ * other.q_ / std::pow(dist, 3) * (pos_ - other.pos_) * 10000;
		//	speed_ += accel * TimeManager::GetDeltaTimeF();
		//	other.speed_ -= accel * TimeManager::GetDeltaTimeF();
		//}
		if (Collider::IsCollide(&collider_, &other.collider_)) {
			//std::cout << "Collided!\n";
			Vector2F line = (_pos - other._pos).Normalized();
			float a = Vector2F::ScalarMult(line, speed_ - other.speed_);
			float distance = (_pos - other._pos).Magnitude() - mass_ * 5.0 - other.mass_ * 5.0;
			/*
			if (mass_ >= other.mass_)
				other.pos_ += line * distance;
			else 
				pos_ += line * distance;
			*/
			float p1 = mass_ * speed_.Magnitude();
			float p2 = other.mass_ * other.speed_.Magnitude();
			_pos -= line * distance / (p1 + p2) * p1;
			other._pos += line * distance / (p1 + p2) * p2;
			
			if (a < EPS) {
				//std::cout << "applied!\n";
				Vector2F dp = Vector2F::ScalarMult((1 + bounce_factor * other.bounce_factor) * (speed_ - other.speed_) / (1.f / mass_ + 1.f / other.mass_), line) * line;
				speed_ -= dp / mass_;
				other.speed_ += dp / other.mass_;
			}
		}
	}

	//! Создание абсолютно нового объекта этого типа
	GameObject* Clone() const {
		return new Circle;
	}

	void AddForce(Vector2F force) {
		speed_ += force / mass_ * TimeManager::GetDeltaTimeF();
	}

	Vector2F GetSpeed() {
		return speed_;
	}
	void SetSpeed(Vector2F speed) {
		speed_ = speed;
	}
private:
	CircleCollider collider_;
	Vector2F speed_;
	float mass_, q_;
	float bounce_factor = 1.f;
	Color color = Color::White();
};

class Connector
{
public:
	virtual void ApplyConnection() = 0;
	void Draw() {
		Debugger::DrawLine(A->GetPos(), B->GetPos(), thick, 0, color);
	}
protected:
	float len = 0;
	Circle* A = nullptr, *B = nullptr;
	Color color = Color::White();
	float thick = 0;
};

class SolidConnector : public Connector
{
public:
	SolidConnector(Circle* a, Circle* b, float l) {
		len = l;
		A = a;
		B = b;
		color = Color::White();
		thick = 2;
	}
	void ApplyConnection() override {
		Vector2F con = A->GetPos() - B->GetPos();
		float d = con.Magnitude();
		Vector2F n = con / d;
		if (std::abs(d - len) > 1) {
			Vector2F cent_speed = (A->GetSpeed() + B->GetSpeed()) / 2;
			float dist = d - len;
			A->SetPos(A->GetPos() - n * dist / 2);
			B->SetPos(B->GetPos() + n * dist / 2);
			A->SetSpeed(A->GetSpeed() + n * Vector2F::ScalarMult(cent_speed - A->GetSpeed(), n));
			B->SetSpeed(B->GetSpeed() + n * Vector2F::ScalarMult(cent_speed - B->GetSpeed(), n));
		}
	}
};

class ElasticConnector : public Connector
{
public:
	ElasticConnector(Circle* a, Circle* b, float l, float k) {
		len = l;
		A = a;
		B = b;
		color = Color::Green();
		thick = 2;
		k_ = k;
	}
	void ApplyConnection() override {
		Vector2F con = A->GetPos() - B->GetPos();
		float d = con.Magnitude();
		Vector2F n = con / d;
		if (std::abs(d - len) > 1) {
			float dist = d - len;
			A->AddForce(-k_ * dist * n);
			B->AddForce(+k_ * dist * n);
		}
	}
private:
	float k_;
};

class MinMaxConnector : public Connector
{
public:
	MinMaxConnector(Circle* a, Circle* b, float lmin, float lmax) {
		l_min = lmin;
		l_max = lmax;
		A = a;
		B = b;
		color = Color::Blue();
		thick = 2;
	}
	void ApplyConnection() override {
		Vector2F con = A->GetPos() - B->GetPos();
		float d = con.Magnitude();
		Vector2F n = con / d;
		if (d - l_min < 0) {
			Vector2F cent_speed = (A->GetSpeed() + B->GetSpeed()) / 2;
			float dist = d - l_min;
			A->SetPos(A->GetPos() - n * dist / 2);
			B->SetPos(B->GetPos() + n * dist / 2);
			A->SetSpeed(A->GetSpeed() + n * Vector2F::ScalarMult(cent_speed - A->GetSpeed(), n));
			B->SetSpeed(B->GetSpeed() + n * Vector2F::ScalarMult(cent_speed - B->GetSpeed(), n));
		}
		else if (l_max - d < 0) {
			Vector2F cent_speed = (A->GetSpeed() + B->GetSpeed()) / 2;
			float dist = d - l_max;
			A->SetPos(A->GetPos() - n * dist / 2);
			B->SetPos(B->GetPos() + n * dist / 2);
			A->SetSpeed(A->GetSpeed() + n * Vector2F::ScalarMult(cent_speed - A->GetSpeed(), n));
			B->SetSpeed(B->GetSpeed() + n * Vector2F::ScalarMult(cent_speed - B->GetSpeed(), n));
		}
	}
private:
	float l_min, l_max;
};

class PhysicsScene : public Scene
{
public:
	void Init() {
		for (int i = 0; i < 1000; i++) {
			Circle a;
			a.Init({ (float)(rand() % 1280), (float)(rand() % 720) }, { 0, 0 }, 2, 2);
			//objs_.push_back(a);
		}
		active_mass = 4;
		active_type = 1;
		active_q = 0;
		GraphicManager::ShowFPS(true);

		//Circle* A = new Circle, * B = new Circle;
		//A->Init({ 400, 400 }, { 0, 0 }, 4, 1);
		//B->Init({ 500, 400 }, { 0, 0 }, 4, 1);
		//objs_.push_back(A);
		//objs_.push_back(B);

		//cons_.push_back(new ElasticConnector(objs_[0], objs_[1], 100, 5000));
	}
	void Update() {

		if (InputManager::IsPressed(KeyboardKey::R)) {
			Destroy();
			cons_.resize(0);
			objs_.resize(0);
		}

		if (InputManager::IsDown(KeyboardKey::LSHIFT)) {
			if (InputManager::IsPressed(KeyboardKey::UP)) {
				active_q += 1;
			}

			if (InputManager::IsPressed(KeyboardKey::DOWN)) {
				active_q -= 1;
			}
		}
		else {
			if (InputManager::IsPressed(KeyboardKey::UP)) {
				active_mass += 1;
			}

			if (InputManager::IsPressed(KeyboardKey::DOWN) && active_mass > 1) {
				active_mass -= 1;
			}
		}
		if (InputManager::IsPressed(MouseKey::Mouse_Left)) {
			pos = InputManager::GetMousePos();
		}

		if (InputManager::IsDown(MouseKey::Mouse_Left)) {
			Debugger::DrawLine(pos, InputManager::GetMousePos(), 2);
		}

		if (InputManager::IsPressed(KeyboardKey::N1))
			active_type = 1;
		if (InputManager::IsPressed(KeyboardKey::N2))
			active_type = 2;
		if (InputManager::IsPressed(KeyboardKey::N3))
			active_type = 3;
		if (InputManager::IsPressed(KeyboardKey::N4))
			active_type = 4;

		if (InputManager::IsRealesed(MouseKey::Mouse_Left)) {
			Circle* a = new Circle;
			a->Init(pos, (InputManager::GetMousePos() - pos) * 3, active_mass, active_type, active_q);
			objs_.push_back(a);
		}

		time += TimeManager::GetDeltaTimeF();
		
		if (time >= 0.02 && objs_.size() < 1000) {
			Circle* a = new Circle;
			Circle* b = new Circle;
			int x = rand() % 1280;
			a->Init({ (float)(x), 0.f }, { 0, 0 }, 1, 1);
			b->Init({ (float)(x + 25), 0.f }, { 0, 0 }, 1, 1);
			objs_.push_back(a);
			objs_.push_back(b);
			cons_.push_back(new ElasticConnector(a, b, 25, 500));
			time = 0;
		}
		
		for (size_t i = 0; i < objs_.size(); i++) {
			objs_[i]->Update();
		}
		for (int k = 0; k < 1; k++) {
			for (size_t i = 0; i < objs_.size(); i++) {
				for (size_t j = i + 1; j < objs_.size(); j++) {
					objs_[i]->TestCollision(*objs_[j]);
				}
			}
		}

		for (int i = 0; i < cons_.size(); i++) {
			cons_[i]->ApplyConnection();
			cons_[i]->Draw();
		}

		for (size_t i = 0; i < objs_.size(); i++) {
			objs_[i]->Draw();
		}
	}
	void Destroy() {
		for (int i = 0; i < cons_.size(); i++)
			delete cons_[i];
		for (int i = 0; i < objs_.size(); i++)
			delete objs_[i];
	}

private:
	std::vector<Circle*> objs_;
	std::vector<Connector*> cons_;
	Vector2F pos;

	float active_mass;
	int active_type;
	int active_q;
	float time = 0;
};