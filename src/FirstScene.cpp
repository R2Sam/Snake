#include "FirstScene.h"

#include "Engine/Components.h"

#include "Raylib/raylib.h"
#include <cstdio>
#include <unordered_set>

FirstScene::FirstScene(const Context& context) :
Scene(context),
_grid(context)
{
	InitAudioDevice();

	_pickupSound = LoadSound("pickup.wav");
	_dieSound = LoadSound("die.wav");
}

FirstScene::~FirstScene() 
{
	CloseAudioDevice();
}

void FirstScene::Update(const float deltaT)
{
	if (IsKeyPressed(KEY_ESCAPE))
	{
		_context.dispatcher.trigger<Event::CloseGame>();
	}

	if(_snake)
	{
		for (; _foodSpawned < _maxFood; _foodSpawned++)
		{
			if (!SpawnFood())
			{
				Log("Won");

				Start();
			}
		}

		u32 currentSnakeSize = _snake->GetSize();
		if (currentSnakeSize > _snakeSize)
		{
			_snakeSize = currentSnakeSize;

			_foodSpawned--;

			_score += 10 * (int(_snakeSize / 5) + 1);
			_maxFood = (int(_snakeSize / 10) + 1);

			PlaySound(_pickupSound);
		}

		if (!_snake->Update(deltaT))
		{
			Log("Lost");

			PlaySound(_dieSound);

			Start();
		}
	}
}

void FirstScene::Draw()
{
	char buffer[32];
	sprintf(buffer, "Score: %u Hi: %u", _score, _bestScore);
	DrawText(buffer, 5, 5, 25, WHITE);
}

void FirstScene::OnEnter()
{
	Start();
}

void FirstScene::OnExit()
{

}

void FirstScene::Start() 
{
	_grid.Reset();

	_snake.emplace(_grid);

	if (_score > _bestScore)
	{
		_bestScore = _score;
	}
	_score = 0;

	_snakeSize = _snake->GetSize();
	_foodSpawned = 0;
	_maxFood = 1;
}

bool FirstScene::SpawnFood() 
{
	std::set<Vector2i> positions;

	while(positions.size() < _grid.GetSize() * _grid.GetSize())
	{
		Vector2i position = {GetRandomValue(0, _grid.GetSize() - 1), GetRandomValue(0, _grid.GetSize() - 1)};
		positions.emplace(position);

		if (_grid.IsSnake(position))
		{
			continue;
		}

		_grid.SpawnFood(position);

		return true;
	}

	return false;
}