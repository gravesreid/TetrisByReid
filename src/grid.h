#ifndef GRID_IS_INCLUDED
#define GRID_IS_INCLUDED
#include <array>
#include "shapes.h"


class Grid
{
public:
    int windowW, windowH, gridW, gridH, r, c;
    int cellSize;
    int curLevel;
    int curScore;
    int linesCleared;
    int totalLinesCleared;
    std::array<std::array<int, 4>, 4> currentShapeArray;
    Color currentShapeColor;
    Position currentShapePosition;
    Shapes currentShape;
    Shapes nextShape;
    std::array<std::array<int, 4>, 4> nextShapeArray;
    Color nextShapeColor;
    Position nextShapePosition;

    void Initialize(int windowWidth, int windowHeight, int gridWidth, int gridHeight, int rows=20, int cols=10);
    void Draw(void);
    void CheckCollision(Shapes &shape);
    bool GetInput();
    void StepGame(float deltaTime);

private:
    std::array<int, 200> gridState{0};
    float dropIntervalMS;
    float dropTimer;
    void UpdateGridState();
    bool CanMove(Shapes &shape, int dRow, int dCol);
};

#endif