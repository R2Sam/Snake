#pragma once

#include "Engine/Context.h"

#include "MyMath/MyVectors.h"

class Grid;

class Snake
{
public:

	Snake(Grid& grid);

	bool Update(const float deltaT);

	u32 GetSize();

private:

	void Input();

	void Grow();

	void MoveSnake(const Vector2i position);

	void SanitizePosition(Vector2i& position);

	u32 RandomSign();

private:

	Grid& _grid;

	Vector2i _direction;

	std::deque<Vector2i> _positions;

	float _speed = 2.5;
	float _speedIncrese = 0.25;
};