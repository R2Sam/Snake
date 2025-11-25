#include "Snake.h"

#include "Grid.h"
#include "MyMath/MyVectors.h"

Snake::Snake(Grid& grid) :
_grid(grid) 
{
	Vector2i headPosition = {GetRandomValue(1, _grid.GetSize() - 1), GetRandomValue(1, _grid.GetSize() -1)};
	_grid.SpawnSnake(headPosition);
	_positions.push_back(headPosition);

	Vector2i tailPosition = headPosition;

	if (GetRandomValue(0, 1))
	{
		tailPosition.x += RandomSign();
	}

	else
	{
		tailPosition.y += RandomSign();
	}

	_grid.SpawnSnake(tailPosition);
	_positions.push_back(tailPosition);

	_direction = headPosition - tailPosition;

	tailPosition -= _direction;
	SanitizePosition(tailPosition);
	_grid.SpawnSnake(tailPosition);
	_positions.push_back(tailPosition);
}

bool Snake::Update(const float deltaT) 
{
	Input();

	Vector2i headPosition = _positions.front();
	Vector2i nextPosition = headPosition + _direction;
	SanitizePosition(nextPosition);

	static float accumulator = 0;
	accumulator += deltaT;

	if (accumulator >= 1.0 / _speed)
	{
		if (_grid.IsSnake(nextPosition) && !(nextPosition.x == _positions.back().x && nextPosition.y == _positions.back().y))
		{
			return false;
		}

		else if (_grid.IsFood(nextPosition))
		{
			Grow();
		}

		else
		{
			MoveSnake(nextPosition);
		}

		accumulator = 0;
	}

	return true;
}

u32 Snake::GetSize()
{
	return _positions.size();
}

void Snake::Input() 
{
	if (IsKeyPressed(KEY_UP) && _direction.y != 1)
	{
		_direction = Vector2i{0, -1};
	}

	if (IsKeyPressed(KEY_DOWN) && _direction.y != -1)
	{
		_direction = Vector2i{0, 1};
	}

	if (IsKeyPressed(KEY_RIGHT) && _direction.x != -1)
	{
		_direction = Vector2i{1, 0};
	}

	if (IsKeyPressed(KEY_LEFT) && _direction.x != 1)
	{
		_direction = Vector2i{-1, 0};
	}
}

void Snake::Grow() 
{
	Vector2i newHeadPosition = _positions.front() + _direction;

	_grid.ClearCell(newHeadPosition);
	_grid.SpawnSnake(newHeadPosition);
	_positions.push_front(newHeadPosition);

	_speed += _speedIncrese;
}

void Snake::MoveSnake(const Vector2i position) 
{
	Vector2i tail = _positions.back();
	_positions.pop_back();
	_grid.ClearCell(tail);

	_grid.SpawnSnake(position);
	_positions.push_front(position);
}

void Snake::SanitizePosition(Vector2i& position) 
{
	position.x = (position.x + _grid.GetSize()) % _grid.GetSize();
	position.y = (position.y + _grid.GetSize()) % _grid.GetSize();
}

u32 Snake::RandomSign() 
{
	return GetRandomValue(0, 1) ? -1 : 1;
}