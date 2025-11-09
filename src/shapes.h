#ifndef SHAPES_H_INCLUDED
#define SHAPES_H_INCLUDED
#include <array>
#include <stdio.h>
#include <vector>

enum class ShapeType
{
    I, J, L, O, S, T, Z
};

struct Position
{
    int row;
    int col;
};

struct Color
{
    int r;
    int g;
    int b;
};

class Shapes
{
private:
    Color color;
    int rotationState;
    Position position;
    ShapeType shapeType;
    std::vector<std::array<std::array<int, 4>, 4>> rotations; // 4 rotation states, each 4x4 grid
public:
    void Initialize();
    void Rotate();
    void MoveDown();
    void MoveLeft();
    void MoveRight();
    Color GetColor() const { return color; }
    Position GetPosition() const { return position; }
    std::array<std::array<int, 4>, 4> GetCurrentRotation() const
    {
        return rotations[rotationState];
    }
    int GetShapeType() const
    {
        return static_cast<int>(shapeType);
    }
};

#endif