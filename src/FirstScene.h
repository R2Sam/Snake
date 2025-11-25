#pragma once

#include "Engine/Context.h"

#include "Grid.h"
#include "Snake.h"

#include <optional>

class FirstScene : public Scene
{
public:

	FirstScene(const Context& context);
	~FirstScene();

	void Update(const float deltaT);
	void Draw();

	void OnEnter();
	void OnExit();

private:

	void Start();
	bool SpawnFood();

private:

	Grid _grid;

	std::optional<Snake> _snake;
	u32 _snakeSize = 0;
	bool _foodSpawned = false;

	u32 _score = 0;
	u32 _bestScore = 0;

	Sound _pickupSound;
	Sound _dieSound;
};