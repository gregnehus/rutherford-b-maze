#include "main.h"

/**********************************************************************************
* Preprocessor Definitions
*********************************************************************************/
//Motors, sensors and controls
#pragma config(Sensor, S1, touchSensor, sensorTouch)
#pragma config(Sensor, S4, sonarSensor, sensorSONAR)
#pragma config(Motor, motorA,  rightMotor, tmotorNormal, PIDControl, encoder)
#pragma config(Motor, motorB,  turretMotor, tmotorNormal, PIDControl, encoder)
#pragma config(Motor, motorC,  leftMotor, tmotorNormal, PIDControl, encoder)

// Maze information
#define MAZE_HEIGHT 5
#define MAZE_WIDTH 6
#define MAZE_ORIGIN_X 1
#define MAZE_ORIGIN_Y 0
#define MAZE_GOAL_X 4
#define MAZE_GOAL_Y 4

// Defines for motor movement
#define DURATION_TURN_90 410
#define DURATION_LOOK_90 400
#define DURATION_DASH_CELL 3500



/**********************************************************************************
* Global Variables
*********************************************************************************/
walls direction_of_travel; //This keeps track of the direction that the bot is travelling
int turret_angle = 0;


task main()
{

    curr_position.x = MAZE_ORIGIN_X;
    curr_position.y = MAZE_ORIGIN_Y;
    align_turret();

    eraseDisplay();
    nxtDisplayStringAt(0, 10, "Hello my name is:");
    nxtDisplayStringAt(0, 15, "Rutherford B Maze");





    // This loops until the bot solves the maze
    while(true){
        maze[curr_position.x][curr_position.y].visited = true;
        scan_cell();
        direction_of_travel = choose_best_cell();        

        if (direction_of_travel == west){
            turn_left(DURATION_TURN_90);
            dash(DURATION_DASH_CELL);
            turn_right(DURATION_TURN_90);

            }else if(direction_of_travel == east){
            turn_right(DURATION_TURN_90);
            dash(DURATION_DASH_CELL);
            turn_left(DURATION_TURN_90);

            }else if(direction_of_travel == south){
            turn_right(2 * DURATION_TURN_90);
            dash(DURATION_DASH_CELL);
            turn_left(2 * DURATION_TURN_90);
            }else{
            dash(DURATION_DASH_CELL);
        }



        nxtDisplayCenteredTextLine(2, "Coord: %d,%d", curr_position.x, curr_position.y);




        //choose_best_move();

        /* Quit if destination found */
        if(curr_position.x == MAZE_GOAL_X && curr_position.y == MAZE_GOAL_Y){
            break;
        }
    }//end while


}//end main


void halt()
{
    motor[rightMotor] = 0;
    motor[leftMotor] = 0;
    motor[turretMotor] = 0;
    wait1Msec(250);
    return;
}

void set_turret_angle(int angle){
    if (angle == turret_angle) return;
    angle = angle - turret_angle;
    turret_angle = angle;
    turn(abs(angle), angle/(abs(angle)));

    

}

void turn(int angle, int direction)
{
    int extra = 0;
    if (turret_angle == 0 || turret_angle == 270) extra = 100;
    halt();
    motor[rightMotor] = 50 * (direction * -1);
    motor[leftMotor] = 50 * direction;
    wait1Msec(DURATION_TURN_90 * (angle / 90) + extra);
    halt();
    return;
}

void turn_left(int duration)
{
    halt();
    motor[rightMotor] = 50;
    motor[leftMotor] = -50;
    wait1Msec(duration);
    halt();
    return;
}

void turn_right(int duration)
{
    halt();
    motor[rightMotor] = -50;
    motor[leftMotor] = 50;
    wait1Msec(duration);
    halt();
    return;
}

void look_left(int duration)
{
    halt();
    motor[turretMotor] = -25;
    wait1Msec(duration);
    halt();
    return;
}

void look_right(int duration)
{
    halt();
    motor[turretMotor] = 25;
    wait1Msec(duration);
    halt();
    return;
}

void dash(int duration)
{
    halt();
    motor[rightMotor] = 50;
    motor[leftMotor] = 50;
    wait1Msec(duration);
    halt();
    return;
}

void print_sonar()
{
    int i, sonarValue;
    for(i=0; i<5; i++){
        sonarValue = SensorValue(sonarSensor); // Store Sonar Sensor values in 'sonarValue' variable.
        nxtDisplayCenteredTextLine(0, "Sonar Reading");  /* Display Sonar Sensor values */
        nxtDisplayCenteredBigTextLine(2, "%d", sonarValue);  /* to LCD screen using %d. */
        wait1Msec(500);
    }
}

int get_sonar()
{
    int sonar = SensorValue(sonarSensor);
    return sonar; // Store Sonar Sensor values in 'sonarValue' variable.


}

void align_turret(){
    look_left(100+ 2 * DURATION_LOOK_90);

}

void scan_cell()
{
    wait1Msec(250);
    //maze[curr_position.x][curr_position.y].cell_walls = 0;

    look_left(100+ DURATION_LOOK_90);

    if (!(maze[curr_position.x][ curr_position.y].cell_walls & west)){
        if (get_sonar() < 60){
            maze[curr_position.x][curr_position.y].cell_walls |= west;
            if (curr_position.x > 0)
            maze[curr_position.x - 1][curr_position.y].cell_walls |= east;
        }
    }

    look_right(100+3*DURATION_LOOK_90);
    if (!(maze[curr_position.x][ curr_position.y].cell_walls & south)){
        if (get_sonar() < 60){

            maze[curr_position.x][curr_position.y].cell_walls |= south;
            if (curr_position.y > 0)
            maze[curr_position.x][curr_position.y - 1].cell_walls |= north;
        }
    }
    look_left(DURATION_LOOK_90);
    if (!(maze[curr_position.x][ curr_position.y].cell_walls & east)){
        if (get_sonar() < 60){


            maze[curr_position.x][curr_position.y].cell_walls |= east;

            if (curr_position.x < MAZE_WIDTH - 1)
            maze[curr_position.x + 1][curr_position.y].cell_walls |= west;

        }
    }
    look_left(DURATION_LOOK_90);
    if (!(maze[curr_position.x][ curr_position.y].cell_walls & north)){
        if (get_sonar() < 60){

            maze[curr_position.x][curr_position.y].cell_walls |= north;

            if (curr_position.y < MAZE_HEIGHT - 1)
            maze[curr_position.x][curr_position.y + 1 ].cell_walls |= south;
        }
    }
    wait1Msec(1250);
    eraseDisplay();
    nxtDisplayCenteredBigTextLine(2, "%d", maze[curr_position.x][curr_position.y].cell_walls);
    return;
}

walls choose_best_cell()
{

    char wallie = maze[curr_position.x][curr_position.y].cell_walls;
    int x;
    int bX, bY;
    int bvX, bvY;
    float shortestUnvisitedDistance = 65535;
    float shortestVisitedDistance = 65535;
    walls dir, vdir;
    unsigned char mask = 1;
    for (x = 0; x<4; x++){
        if (mask & ~wallie){
            coord n;
            get_neighbor_coordinate(curr_position.x, curr_position.y, (walls)mask, n);
            if (n.x < 0 || n.y < 0 || n.x >= MAZE_WIDTH || n.y >= MAZE_HEIGHT) continue;

            if (get_distance(curr_position.x, curr_position.y, n.x, n.y) < shortestUnvisitedDistance && !maze[n.x][n.y].visited){
                shortestUnvisitedDistance = get_distance(curr_position.x, curr_position.y, n.x, n.y);
                bX = n.x;
                bY = n.y;
                dir = (walls)mask;
            }
            if (get_distance(curr_position.x, curr_position.y, n.x, n.y) < shortestVisitedDistance && maze[n.x][n.y].visited){
                shortestVisitedDistance = get_distance(curr_position.x, curr_position.y, n.x, n.y);
                bvX = n.x;
                bvY = n.y;
                vdir = (walls)mask;
            }

        }
        mask = mask << 1;


    }

    if (shortestUnvisitedDistance == 65535){
        PlaySound(soundBlip);
        wait1Msec(250);
        PlaySound(soundBlip);
        curr_position.x = bvX;
        curr_position.y = bvY;
        walls oppV;

        if (vdir == north){
            oppV = south;
            }else if (vdir == east){
            oppV = west;

            }else if (vdir == west){
            oppV = east;
            }else{
            oppV = north;
        }
        maze[curr_position.x][curr_position.y].cell_walls |= oppV;
        return vdir;

    }
    PlaySound(soundBlip);

    curr_position.x = bX;
    curr_position.y = bY;
    return dir;
}

float get_distance(int x1, int y1, int x2, int y2){
    return sqrt( (x2 - x1)* (x2 - x1) + (y2 - y1)* (y2 - y1));

}

void get_neighbor_coordinate(int x, int y, walls dir, coord *n){

    if (dir == west){
        n->x = x - 1;
        n->y = y;

        }else if(dir == north){
        n->x = x;
        n->y = y + 1;
        }else if(dir == east){
        n->x = x + 1;
        n->y = y;
        }else if(dir == south){
        n->x = x;
        n->y = y -1;
    }


}