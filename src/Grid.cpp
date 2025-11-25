#include "Grid.h"

#include "Assert.h"

#include "Engine/Components.h"
#include "Components.h"

#include "Raylib/raylib.h"

Grid::Grid(const Context& context) :
_context(context)
{
	Assert(GetScreenWidth() == GetScreenHeight(), "Window must be square");

	_size = 10;

	Assert(fmod(GetScreenWidth(), _size) <= 1, "Grid does not allign with window size");
	_gridSize = GetScreenWidth() / _size;
	_grid.resize(_size, _size, entt::null);

	Image snakeImage = GenImageColor(_gridSize, _gridSize, BLANK);
	ImageDrawRectangleRounded(&snakeImage, {2.5, 2.5, _gridSize - 2.5, _gridSize - 2.5}, 0.2, GREEN);
	_snakeTexture = LoadTextureFromImage(snakeImage);
	UnloadImage(snakeImage);

	Image foodImage = GenImageColor(_gridSize, _gridSize, BLANK);
	ImageDrawCircleV(&foodImage, {_gridSize / 2, _gridSize / 2}, _gridSize / 2, RED);
	_foodTexture = LoadTextureFromImage(foodImage);
	UnloadImage(foodImage);
}

bool Grid::IsSnake(const Vector2i position)
{
	entt::entity entity = _grid[position.x, position.y];

	if (entity == entt::null)
	{
		return false;
	}

	Component::Snake& snake = _context.registry.get<Component::Snake>(entity);
	return snake.isSnake;
}

bool Grid::IsFood(const Vector2i position)
{
	entt::entity entity = _grid[position.x, position.y];

	if (entity == entt::null)
	{
		return false;
	}

	Component::Snake& snake = _context.registry.get<Component::Snake>(entity);
	return snake.isFood;
}

void Grid::SpawnSnake(const Vector2i position)
{
	if (IsSnake(position))
	{
		return;
	}

	entt::entity entity = _context.registry.create();
	_grid[position.x, position.y] = entity;
	_context.registry.emplace<Component::Snake>(entity, true, false);
	_context.registry.emplace<Component::Transform>(entity, Vector2f{position.x * _gridSize + _gridSize / 2, position.y * _gridSize + _gridSize / 2});
	_context.registry.emplace<Component::Sprite>(entity, _snakeTexture, Rectangle{0, 0, _gridSize, _gridSize});
}

void Grid::SpawnFood(const Vector2i position)
{
	if (IsFood(position))
	{
		return;
	}

	entt::entity entity = _context.registry.create();
	_grid[position.x, position.y] = entity;
	_context.registry.emplace<Component::Snake>(entity, false, true);
	_context.registry.emplace<Component::Transform>(entity, Vector2f{position.x * _gridSize + _gridSize / 2, position.y * _gridSize + _gridSize / 2});
	_context.registry.emplace<Component::Sprite>(entity, _foodTexture, Rectangle{0, 0, _gridSize, _gridSize});
}

void Grid::ClearCell(const Vector2i position) 
{
	entt::entity entity = _grid[position.x, position.y];
	_grid[position.x, position.y] = entt::null;

	if (entity != entt::null)
	{
		_context.registry.destroy(entity);
	}
}

u32 Grid::GetSize() 
{
	return _grid.getCols();
}

u32 Grid::GetCellSize() 
{
	return _gridSize;
}

void Grid::Reset()
{
	_grid.clear();
	_grid.resize(_size, _size, entt::null);

	auto view = _context.registry.view<Component::Snake>();

	for (const entt::entity entity : view)
	{
		_context.registry.destroy(entity);
	}
}

void Grid::ImageDrawRectangleRounded(Image* img, Rectangle rec, const float roundness, Color color)
{
    float r = roundness * (rec.width < rec.height ? rec.width : rec.height);

    ImageDrawRectangleRec(img, {rec.x + r, rec.y, rec.width - 2*r, rec.height}, color);
    ImageDrawRectangleRec(img, {rec.x, rec.y + r, rec.width, rec.height - 2*r}, color);

    Vector2 tl = { rec.x + r, rec.y + r };
    Vector2 tr = { rec.x + rec.width - r, rec.y + r };
    Vector2 bl = { rec.x + r, rec.y + rec.height - r };
    Vector2 br = { rec.x + rec.width - r, rec.y + rec.height - r };

    ImageDrawCircleV(img, tl, r, color);
    ImageDrawCircleV(img, tr, r, color);
    ImageDrawCircleV(img, bl, r, color);
    ImageDrawCircleV(img, br, r, color);
}