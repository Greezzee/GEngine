#pragma once
#include "EllipseCollider.h"
#include "SquareCollider.h"
#include "CircleCollider.h"

namespace ge {
	class Collider
	{
	public:
		/*!
		* Получает указатели на 2 эллиптических коллайдера
		* Вернёт false, если данные эллипсы не пересекаются
		* Вернёт true, еслии пересекаются
		*
		* Функция НЕ 100% ТОЧНАЯ, использует приближения. Степени точности определяет COLLIDE_ITERATIONS
		*/
		static bool IsCollide(EllipseCollider* a, EllipseCollider* b);

		//! Проверяет, пересекаются ли 2 данных коллайдера
		static bool IsCollide(SquareCollider* a, SquareCollider* b);

		//! Проверяют, пересекаются ли 2 данных круга
		static bool IsCollide(CircleCollider* a, CircleCollider* b);

		//! Проверяет, пересекаются ли данный круг и данный прямоугольник
		static bool IsCollide(CircleCollider* a, SquareCollider* b);

		//! Проверяет, пересекаются ли данный круг и данный прямоугольник
		static bool IsCollide(SquareCollider* a, CircleCollider* b);

		//! Выпускает из центра коллайдера a вектор в направлении direction
		//! Определяет, сколько должен пройти коллайдер a в направлении данного вектора, чтобы столкнуться в b
		//! Вернёт искомое расстояние в тех же единицах, в которых заданы коллайдеры
		//! Вернёт NAN, если при движении в данном направлении, a никогда не встретит b
		//! НЕ РАБОТАЕТ
		static float DistanceBetween(SquareCollider* a, SquareCollider* b, const Vector2F& direction);

		//! Выпускает из центра коллайдера a вектор в направлении direction
		//! Определяет, сколько должен пройти коллайдер a в направлении данного вектора, чтобы столкнуться в b
		//! Вернёт искомое расстояние в тех же единицах, в которых заданы коллайдеры
		//! Вернёт NAN, если при движении в данном направлении, a никогда не встретит b
		//! НЕ РАБОТАЕТ
		static float DistanceBetween(CircleCollider* a, SquareCollider* b, const Vector2F& direction);

		//! Выпускает из центра коллайдера a вектор в направлении direction
		//! Определяет, сколько должен пройти коллайдер a в направлении данного вектора, чтобы столкнуться в b
		//! Вернёт искомое расстояние в тех же единицах, в которых заданы коллайдеры
		//! Вернёт NAN, если при движении в данном направлении, a никогда не встретит b

		static float DistanceBetween(CircleCollider* a, CircleCollider* b, const Vector2F& direction);

	private:

		//! Проверяет пересечение двух коллайдеров по оси norm
		static bool AxisIntersect(SquareCollider* a, SquareCollider* b, const Vector2F& norm);

		//! Лучше не спрашивайте... И НИКОГДА НЕ ТРОГАЙТЕ
		static Vector2F AxisDistance(SquareCollider* a, SquareCollider* b, const Vector2F& norm, const Vector2F& dir);
	};
}