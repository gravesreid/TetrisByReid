#include <stdlib.h>
#include <stdio.h>
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "grid.h"
#include "shapes.h"


void Grid::Initialize(int windowWidth, int windowHeight, int gridWidth, int gridHeight, int rows, int cols)
{
    windowW = windowWidth;
    windowH = windowHeight;
    gridW = gridWidth;
    gridH = gridHeight;
    r = rows;
    c = cols;
    cellSize = gridW / c;
    // current shape
    currentShape.Initialize();
    currentShapeArray = currentShape.GetCurrentRotation();
    currentShapeColor = currentShape.GetColor();
    currentShapePosition = currentShape.GetPosition();
    // next shape
    nextShape.Initialize();
    nextShapePosition = nextShape.GetPosition();
    nextShapeArray = nextShape.GetCurrentRotation();
    nextShapeColor = nextShape.GetColor();
    // game metrics
    dropIntervalMS = 500.0f; // drop every 500 ms
    dropTimer = 0.0f;
    curLevel = 1;
    curScore = 0;
    linesCleared = 0;
    totalLinesCleared = 0;
}

void Grid::Draw(void)
{
    // draw left half background
    glColor3f(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(windowW/2, 0);
    glVertex2i(windowW/2, windowH);
    glVertex2i(0, windowH);
    glEnd();
    //draw right half background
    glColor3f(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2i(windowW/2, 0);
    glVertex2i(windowW, 0);
    glVertex2i(windowW, windowH);
    glVertex2i(windowW/2, windowH);
    glEnd();
    // Show tetris
    glColor3f(1,1,1);
    glRasterPos2i(10, 50);
    YsGlDrawFontBitmap20x32("TETRIS");
    // Show score
    glRasterPos2i(10, 100);
    YsGlDrawFontBitmap16x24("SCORE:");
    char scoreStr[64];
    sprintf(scoreStr, "%d", curScore);
    YsGlDrawFontBitmap16x24(scoreStr);
    // Show level
    glRasterPos2i(10, 150);
    YsGlDrawFontBitmap16x24("LEVEL:");
    char levelStr[64];
    sprintf(levelStr, "%d", curLevel);
    YsGlDrawFontBitmap16x24(levelStr);
    // Show lines cleared
    glRasterPos2i(10, 200);
    YsGlDrawFontBitmap16x24("LINES:");
    char linesStr[64];
    sprintf(linesStr, "%d", totalLinesCleared);
    YsGlDrawFontBitmap16x24(linesStr);
    // draw boundary
    glColor3f(.821,0.920,.754);
    glBegin(GL_QUADS);
    glVertex2i(windowW/2 - gridW/2,0);
    glVertex2i(windowW/2 + gridW/2,0);
    glVertex2i(windowW/2 + gridW/2,windowH);
    glVertex2i(windowW/2 - gridW/2,windowH);
    glEnd();
    // draw grid lines
    glColor3f(.68, .68, .68);
    for(int i=0; i<=c; ++i)
    {
        glBegin(GL_LINES);
        glVertex2i(windowW/2 - gridW/2 + i*cellSize,0);
        glVertex2i(windowW/2 - gridW/2 + i*cellSize,windowH);
        glEnd();
    }
    for(int j=0; j<=r; ++j)
    {
        glBegin(GL_LINES);
        glVertex2i(windowW/2 - gridW/2,j*cellSize);
        glVertex2i(windowW/2 + gridW/2,j*cellSize);
        glEnd();
    }

    // draw current shape
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (currentShapeArray[i][j] == 1)
            {
                int x = (currentShapePosition.col + j) * cellSize + (windowW/2 - gridW/2);
                int y = (currentShapePosition.row + i) * cellSize;
                glColor3ub(currentShapeColor.r, currentShapeColor.g, currentShapeColor.b);
                glBegin(GL_QUADS);
                glVertex2i(x, y);
                glVertex2i(x + cellSize, y);
                glVertex2i(x + cellSize, y + cellSize);
                glVertex2i(x, y + cellSize);
                glEnd();
                // draw border
                glColor3f(0,0,0);
                glBegin(GL_LINE_LOOP);
                glVertex2i(x, y);
                glVertex2i(x + cellSize, y);
                glVertex2i(x + cellSize, y + cellSize);
                glVertex2i(x, y + cellSize);
                glEnd();
            }
        }
    }
    // draw next shape preview
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (nextShapeArray[i][j] == 1)
            {
                int x = windowW - 250 + j * cellSize;
                int y = 100 + i * cellSize;
                glColor3ub(nextShapeColor.r, nextShapeColor.g, nextShapeColor.b);
                glBegin(GL_QUADS);
                glVertex2i(x, y);
                glVertex2i(x + cellSize, y);
                glVertex2i(x + cellSize, y + cellSize);
                glVertex2i(x, y + cellSize);
                glEnd();
                // draw border
                glColor3f(0,0,0);
                glBegin(GL_LINE_LOOP);
                glVertex2i(x, y);
                glVertex2i(x + cellSize, y);
                glVertex2i(x + cellSize, y + cellSize);
                glVertex2i(x, y + cellSize);
                glEnd();
            }
        }
    }
    // draw box around next shape preview
    glColor3f(1,1,1);
    glBegin(GL_LINE_LOOP);
    glVertex2i(windowW - 250, 75);
    glVertex2i(windowW - 250 + 4*cellSize, 75);
    glVertex2i(windowW - 250 + 4*cellSize, 75 + 4*cellSize);
    glVertex2i(windowW - 250, 75 + 4*cellSize);
    glEnd();
    // draw existing blocks in gridState
    for (int row = 0; row < r; ++row)
    {
        for (int col = 0; col < c; ++col)
        {
            int index = row * c + col;
            if (gridState[index] != 0)
            {
                // Assign color based on stored shape type
                int shapeType = gridState[index];
                Color blockColor;
                switch (shapeType)
                {
                case 1: blockColor = {38, 91, 97}; break; 
                case 2: blockColor = {62, 41, 96}; break;   
                case 3: blockColor = {97, 75, 38}; break;
                case 4: blockColor = {171, 145, 115}; break; 
                case 5: blockColor = {102, 107, 4}; break;   
                case 6: blockColor = {69, 94, 122}; break; 
                case 7: blockColor = {69, 122, 77}; break;   
                default: blockColor = {121, 122, 69}; break;
                }
                glColor3ub(blockColor.r, blockColor.g, blockColor.b);
                int x = col * cellSize + (windowW/2 - gridW/2);
                int y = row * cellSize;
                glBegin(GL_QUADS);
                glVertex2i(x, y);
                glVertex2i(x + cellSize, y);
                glVertex2i(x + cellSize, y + cellSize);
                glVertex2i(x, y + cellSize);
                glEnd();
                // draw border
                glColor3f(0,0,0);
                glBegin(GL_LINE_LOOP);
                glVertex2i(x, y);
                glVertex2i(x + cellSize, y);
                glVertex2i(x + cellSize, y + cellSize);
                glVertex2i(x, y + cellSize);
                glEnd();
            }
        }
    }
}

void Grid::CheckCollision(Shapes &shape)
{
    auto shapeArray = shape.GetCurrentRotation();
    auto shapePos = shape.GetPosition();

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (shapeArray[i][j] == 1)
            {
                int gridRow = shapePos.row + i;
                int gridCol = shapePos.col + j;

                // Check existing blocks in gridState
                if (gridRow >= 0) // Only check if within visible grid
                {
                    int index = (gridRow+1) * c + gridCol;
                    if (gridState[index] != 0)
                    {
                        UpdateGridState();
                        return;
                    }
                }
                // Check if hit the bottom
                if (gridRow == r - 1)
                {
                    UpdateGridState();
                    return;
                }
            }
        }
    }
}

bool Grid::GetInput()
{
    auto key = FsInkey();
    if (key == FSKEY_LEFT && CanMove(currentShape, 0, -1))
    {
        currentShape.MoveLeft();
    }
    else if (key == FSKEY_RIGHT && CanMove(currentShape, 0, 1))
    {
        currentShape.MoveRight();
    }
    else if (key == FSKEY_DOWN)
    {
        currentShape.MoveDown();
    }
    else if (key == FSKEY_UP)
    {
        currentShape.Rotate();
    }
    else if (key == FSKEY_ESC)
    {
        return false;
    }

    // Update current shape properties after movement/rotation
    currentShapeArray = currentShape.GetCurrentRotation();
    currentShapeColor = currentShape.GetColor();
    currentShapePosition = currentShape.GetPosition();
    return true;
}

void Grid::UpdateGridState()
{
    // add current shape to gridState when it lands
    auto shapeArray = currentShape.GetCurrentRotation();
    auto shapePos = currentShape.GetPosition();
    auto shapeType = currentShape.GetShapeType();

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (shapeArray[i][j] == 1)
            {
                int gridRow = shapePos.row + i;
                int gridCol = shapePos.col + j;

                if (gridRow >= 0 && gridRow < r && gridCol >= 0 && gridCol < c)
                {
                    int index = gridRow * c + gridCol;
                    gridState[index] = shapeType + 1; // Store shape type + 1 to differentiate from empty
                }
            }
        }
    }
    // Clear full lines
    for (int row = 0; row < r; ++row)
    {
        bool isFull = true;
        for (int col = 0; col < c; ++col)
        {
            int index = row * c + col;
            if (gridState[index] == 0)
            {
                isFull = false;
                break;
            }
        }
        if (isFull)
        {
            // Shift down all rows above
            for (int moveRow = row; moveRow > 0; --moveRow)
            {
                for (int col = 0; col < c; ++col)
                {
                    int fromIndex = (moveRow - 1) * c + col;
                    int toIndex = moveRow * c + col;
                    gridState[toIndex] = gridState[fromIndex];
                }
            }
            // Clear top row
            for (int col = 0; col < c; ++col)
            {
                gridState[col] = 0;
            }
            linesCleared++;
            totalLinesCleared++;
            curScore += 40*curLevel;
        }
    }
    // Initialize new current shape from next shape
    currentShape = nextShape;
    currentShapePosition = currentShape.GetPosition();
    currentShapeArray = currentShape.GetCurrentRotation();
    currentShapeColor = currentShape.GetColor();
    // Generate new next shape
    nextShape.Initialize();
    nextShapePosition = nextShape.GetPosition();
    nextShapeArray = nextShape.GetCurrentRotation();
    nextShapeColor = nextShape.GetColor();
}

void Grid::StepGame(float deltaTime)
{
    dropTimer += deltaTime;
    if (dropTimer >= dropIntervalMS)
    {
        currentShape.MoveDown();
        dropTimer = 0.0f;

        // Update current shape properties after moving down
        currentShapeArray = currentShape.GetCurrentRotation();
        currentShapePosition = currentShape.GetPosition();
        currentShapeColor = currentShape.GetColor();
        CheckCollision(currentShape);
        if (linesCleared > 10)
        {
            curLevel++;
            linesCleared = 0;
            dropIntervalMS *= 0.9f; // Increase speed by reducing drop interval
        }
    }
}

bool Grid::CanMove(Shapes &shape, int dRow, int dCol)
{
    auto shapeArray = shape.GetCurrentRotation();
    auto shapePos = shape.GetPosition();

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (shapeArray[i][j] == 1)
            {
                int newRow = shapePos.row + i + dRow;
                int newCol = shapePos.col + j + dCol;

                // Check boundaries
                if (newCol < 0 || newCol >= c || newRow >= r)
                {
                    return false;
                }

                // Check existing blocks in gridState
                if (newRow >= 0) // Only check if within visible grid
                {
                    int index = newRow * c + newCol;
                    if (gridState[index] != 0)
                    {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}