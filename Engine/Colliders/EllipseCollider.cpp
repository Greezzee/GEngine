#include "../Colliders/EllipseCollider.h"
#include <cmath>
using namespace ge;
const float PI = 3.1415926f;

const unsigned EllipseCollider::COLLIDE_ITERATIONS = 16;

void EllipseCollider::Init()
{
	_pos = _size = { 0, 0 };
	_angle = 0;
}

void EllipseCollider::Init(Vector2F pos, float angle, Vector2F size)
{
	_pos = pos;
	_size = size;
	_angle = angle;
}