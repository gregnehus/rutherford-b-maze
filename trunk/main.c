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
directions turret_angle = dSouth;
directions base_angle = dNorth;
directions dir_lookup[] =
      {0,dWest,dNorth,0,dEast,0,0,0,dSouth};
walls opp_wall_lookup[]=
      {0,east, south, 0,west, 0, 0, 0, north};

cell maze[MAZE_WIDTH][MAZE_HEIGHT];

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

        set_base_angle(dNorth);



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

void set_base_angle(directions d_angle){
    if (d_angle == base_angle) return;
    int angle = (int) d_angle;
    angle = angle - base_angle;
    base_angle = (directions)angle;
    turn_base(abs(angle), angle/(abs(angle)));



}


void set_turret_angle(directions d_angle){
    if (d_angle == turret_angle) return;
    int angle = (int) d_angle;
    angle = angle - turret_angle;
    turret_angle = (directions)angle;
    turn_turret(abs(angle), angle/(abs(angle)));



}

void turn_turret(int angle, int direction)
{
    halt();
    int fineTune = 0;
    if (angle == 0 || angle == 270) fineTune = 100;
    motor[turretMotor] = 25 * direction;
    wait1Msec(DURATION_LOOK_90 * (angle / 90) + fineTune);
    halt();
    return;
}

void turn_base(int angle, int direction)
{

    halt();
    motor[rightMotor] = 50 * (direction * -1);
    motor[leftMotor] = 50 * direction;
    wait1Msec(DURATION_TURN_90 * (angle / 90));
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



int get_sonar()
{
    int sonar = SensorValue(sonarSensor);
    /*
    nxtDisplayCenteredTextLine(0, "Sonar Reading");
    nxtDisplayCenteredBigTextLine(2, "%d", sonar);
    */
    return sonar; // Store Sonar Sensor values in 'sonarValue' variable.


}

void align_turret(){
    set_turret_angle(dWest);

}

void scan_wall(walls w){
    coord n;
    if (maze[curr_position.x][curr_position.y].cell_walls & w) return;


    set_turret_angle(dir_lookup[(int) w]);
    if (get_sonar() < 60){
            maze[curr_position.x][curr_position.y].cell_walls |= w;
            if (get_neighbor_coordinate(curr_position.x, curr_position.y, w, n))
                maze[n.x][n.y].cell_walls |= (walls) opp_wall_lookup[(int) w];

     }

}
void scan_cell()
{
    scan_wall(west);
    scan_wall(south);
    scan_wall(east);
    scan_wall(north);

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

            if (!get_neighbor_coordinate(curr_position.x, curr_position.y, (walls)mask, n)) continue;

            if (get_distance(curr_position.x, curr_position.y, n.x, n.y) < shortestUnvisitedDistance && !maze[n.x][n.y].visited){
                shortestUnvisitedDistance = get_distance(curr_position.x, curr_position.y, n.x, n.y);
                bX = n.x;
                bY = n.y;
                dir = (walls)mask;
            }else if (get_distance(curr_position.x, curr_position.y, n.x, n.y) < shortestVisitedDistance && maze[n.x][n.y].visited){
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

        oppV = (walls) opp_wall_lookup[(int) vdir];
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

bool get_neighbor_coordinate(int x, int y, walls dir, coord *n){

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
        if (n->x < 0 || n->y < 0 || n->x >= MAZE_WIDTH  || n->y >= MAZE_HEIGHT) return false;
        return true;


}
