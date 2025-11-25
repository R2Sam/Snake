#pragma once

#include "Engine/Context.h"

#include "MyMath/MyVectors.h"
#include "MyMath/MyMatrix.h"

class Grid
{
public:

	Grid(const Context& context);

	bool IsSnake(const Vector2i position);
	bool IsFood(const Vector2i position);

	void SpawnSnake(const Vector2i position);
	void SpawnFood(const Vector2i position);
	void ClearCell(const Vector2i position);

	u32 GetSize();
	u32 GetCellSize();

	void Reset();

private:

	void ImageDrawRectangleRounded(Image* img, Rectangle rec, const float roundness, Color color);

private:

	const Context& _context;

	Matrix2D<entt::entity> _grid;
	u32 _size;
	u32 _gridSize;

	Texture2D _snakeTexture;
	Texture2D _foodTexture;
};