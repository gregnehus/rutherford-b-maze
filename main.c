#define MAZE_HEIGHT 5
#define MAZE_WIDTH 6
/* INCLUDES */
#include "main.h"
//#include "RBMaze.h"


#define MAZE_ORIGIN_X 0
#define MAZE_ORIGIN_Y 1


#define MAZE_GOAL_X 4
#define MAZE_GOAL_Y 4


/* VARS */



/* MAIN */
task main()
{
  /* init */
curr_position.x = MAZE_ORIGIN_X;
curr_position.y = MAZE_ORIGIN_Y;

  eraseDisplay();
  nxtDisplayStringAt(0, 10, "Hello my name is:");
  nxtDisplayStringAt(0, 15, "Rutherford B Maze");
  //halt();

  /* while loop */
  while(1){
    scan_cell();
    //choose_best_move();

    /* Quit if destination found */
    if(1){
      break;
    }
  }//end while


}//end main
