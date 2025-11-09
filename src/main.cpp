#include <stdio.h>
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "yssimplesound.h"
#include <math.h>
#include <iostream>
#include "shapes.h"
#include "grid.h"
    

int main(void)
{
	FsOpenWindow(50,50,800,600,0);

    Grid grid;
    grid.Initialize(800, 600, 300, 600, 20, 10);

    YsSoundPlayer player;
    player.MakeCurrent();
    player.Start();
    YsSoundPlayer::SoundData backgroundMusic;
    backgroundMusic.LoadWav("../audio/tetris.wav");

    for(;;)
	{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        grid.Draw();
        grid.GetInput();
        grid.CheckCollision(grid.currentShape);
        int t = FsPassedTime();
        grid.StepGame(static_cast<float>(t));
        FsSwapBuffers();
		FsPollDevice();
	    glFlush();
		FsSleep(20);
	player.PlayBackground(backgroundMusic);
	}
	return 0;
}