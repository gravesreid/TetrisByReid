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
	FsOpenWindow(75,75,1200,900,0);

    Grid grid;
    grid.Initialize(1200, 900, 450, 900, 20, 10);

    YsSoundPlayer player;
    player.MakeCurrent();
    player.Start();
    YsSoundPlayer::SoundData backgroundMusic;
    backgroundMusic.LoadWav("../audio/tetris.wav");

    bool running = true;

    while(running)
	{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        grid.Draw();
        running =grid.GetInput();
        grid.CheckCollision(grid.currentShape);
        int t = FsPassedTime();
        grid.StepGame(static_cast<float>(t));
        FsSwapBuffers();
		FsPollDevice();
	    glFlush();
		FsSleep(20);
	player.PlayBackground(backgroundMusic);
	}
    player.Stop(backgroundMusic);
    player.End();
	return 0;
}