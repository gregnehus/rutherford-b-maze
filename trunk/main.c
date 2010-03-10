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
walls direction_of_travel;                      //This keeps track of the direction that the bot is travelling


directions turret_angle = dSouth;               // Variable to hold the direction that turret is facing, set to south so that it is forced
                                                // Far to left at startup to properly align against alignment setup

directions base_angle = dNorth;                 // Variable to store the direction that the base robot is facing

walls wall_lookup[] =                           // Array serves as way to have access to all enumerated wall types
    {0, west, north, 0, east, 0, 0, 0, south};


directions dir_lookup[] =                       // Array serves as easy way to get direction value of wall, for angle references
      {0,dWest,dNorth,0,dEast,0,0,0,dSouth};

walls opp_wall_lookup[]=                        // Array gives easy way to get value of opposing wall direction
      {0,east, south, 0,west, 0, 0, 0, north};

cell maze[MAZE_WIDTH][MAZE_HEIGHT];


/********************************************************************************
 * Function: task main()
 * Parameters: None
 * Return: A task
 * Description: This function contains the main loop of the program
 */

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


/********************************************************************************
 * Function: void halt()
 * Parameters: None
 * Return: None
 * Description: This function stops all motors.
 */

void halt()
{
    motor[rightMotor] = 0;
    motor[leftMotor] = 0;
    motor[turretMotor] = 0;
    wait1Msec(250);
    return;
}

/********************************************************************************
 * Function: set_base_angle
 * Parameters: Takes an enumerated direction name
 * Return: None
 * Description: This function sets the direction that the robot should face
 */

void set_base_angle(directions angle){

    directions r_angle;
    if (angle == base_angle) return;                        // If the angle that is requested is the current angle, quit

    r_angle = (directions) ((int)angle - (int)base_angle);    // Adjust the angle relative to where the base is currently aimed



    turn_base(abs(r_angle), r_angle/(abs(r_angle)));              // Turn the base

    base_angle = angle;   // Update the angle at which the base is aimed



}

/********************************************************************************
 * Function: set_turret_angle
 * Parameters: Takes an enumerated direction name
 * Return: None
 * Description: This function figures out the angle that the turret needs to turn and direction
 */
void set_turret_angle(directions angle){

    directions r_angle;

    if (angle == turret_angle) return;                          // If requested angle is already set, just leave
    
    r_angle = (directions) ((int)angle - (int)turret_angle);    // Adjust the angle relative to where the turret is currently aimed
    
    turn_turret(abs(r_angle), r_angle/(abs(r_angle)));                // Turn the turret

    turret_angle = angle;  // Update the angle at which the base is aimed


}
/********************************************************************************
 * Function: turn_turret
 * Parameters: Takes an absolute (as opposed to relative) angle and direction (negative or positive)
 * Return: None
 * Description: This function turns the turret to a particular direction
 */
void turn_turret(int angle, int direction)
{
    halt();                                                 // Stop all motors

    int fineTune = 0;                                       // Variable used to push motor a little further when that angle has physical stoppers
    if (angle == 0 || angle == 270) fineTune = 100;         // There are stoppers at 0 & 270, so lets go a little further for both of these angles
    motor[turretMotor] = 25 * direction;                    // Set turret motor speed
    wait1Msec(DURATION_LOOK_90 * (angle / 90) + fineTune);  // Timer delay
    halt();                                                 // Stop all motors

}

/********************************************************************************
 * Function: turn_base
 * Parameters: Takes an absolute (as opposed to relative) angle and direction (negative or positive)
 * Return: None
 * Description: This function turns the base of the robot
 */
void turn_base(int angle, int direction)
{

    halt();                                                 // Stop all motors
    motor[rightMotor] = 50 * (direction * -1);              // Set right motor speed
    motor[leftMotor] = 50 * direction;                      // Set left motor speed
    wait1Msec(DURATION_TURN_90 * (angle / 90));             // Set delay
    halt();                                                 // Stop all motors

}


/********************************************************************************
 * Function: dash
 * Parameters: Takes an absolute (as opposed to relative) angle and direction (negative or positive)
 * Return: None
 * Description: This function makes the robot dash forward
 */

void dash(int duration)
{
    halt();                                             // Stop all motors
    motor[rightMotor] = 50;                             // Set right motor speed
    motor[leftMotor] = 50;                              // Set left motor speed
    wait1Msec(duration);                                // wait
    halt();                                             // Stop all motors

}


/********************************************************************************
 * Function: get_sonar
 * Parameters: None
 * Return: Integer value of sonar reading
 * Description: Function reads the sonar and returns the filtered reading
 */
int get_sonar()
{
    int sonar = SensorValue(sonarSensor);
    /*
    nxtDisplayCenteredTextLine(0, "Sonar Reading");
    nxtDisplayCenteredBigTextLine(2, "%d", sonar);
    */
    return sonar; // Store Sonar Sensor values in 'sonarValue' variable.


}

/********************************************************************************
 * Function: align_turret
 * Parameters: None
 * Return: None
 * Description: Function turns the turret all the way to the left to make sure it is aligned
 */
void align_turret(){
    set_turret_angle(dWest);

}

/********************************************************************************
 * Function: scan_walls
 * Parameters: Enumerated wall enum
 * Return: None
 * Description: Function scans a wall in a particular direction
 */
void scan_wall(walls w){


    coord n;    //Variable to hold adjacent cell if wall is found


    if (maze[curr_position.x][curr_position.y].cell_walls & w) return;  // If there is already a wall here, don't scan it

    set_turret_angle(dir_lookup[(int) w]); //Turn to the particular wall

    // If there is an object that has a distance of less than 60 mms
    if (get_sonar() < 60){
            maze[curr_position.x][curr_position.y].cell_walls |= w;             // Mark this as a wall

            // If there is a neighboring cell
            if (get_neighbor_coordinate(curr_position.x, curr_position.y, w, n))
                maze[n.x][n.y].cell_walls |= (walls) opp_wall_lookup[(int) w];  //Mark the opposing wall as a wall

     }

}

/********************************************************************************
 * Function: scan_cell
 * Parameters: None
 * Return: None
 * Description: Function a cell in all wall directions
 */
void scan_cell()
{
    int x;
    for (x = 1; x < 9; x = x * 2)                   // This for loop will iterate through wall_lookup array
        scan_wall(wall_lookup[x]);                  // which has all of the wall types in it
      
}


/********************************************************************************
 * Function: choose_best_cell
 * Parameters: None
 * Return: None
 * Description: Function determines which direction that the robot should move in. If there is a best
 *              route that facilitates an unvisited adjacent cell, the robot will travel there, however,
 *              if there is no open cell that is unvisited, the robot will go back to the cell that it had come
 *              from.
 */
walls choose_best_cell()
{


    int x;                                                                      // For loop variable
    int bX, bY;                                                                 // Best coordinates for non-visited route
    int bvX, bvY;                                                               // Best coordinates for visited route

    float shortestUnvisitedDistance = 65535;                                    // Variable to hold best univisted distance
    float shortestVisitedDistance = 65535;                                      // Variable to hold best visited distance


    walls dir, vdir;                                                            // Hold directions of direction of best visited and unvisted cells

    unsigned char mask = 1;                                                     // Bit mask to scan through walls enum

    for (x = 0; x<4; x++){


        if (mask & ~(maze[curr_position.x][curr_position.y].cell_walls)){       // If there is no wall at given direction
            
            coord n;                                                            // Variable to hold proposed next cell

            if (!get_neighbor_coordinate(curr_position.x, curr_position.y, (walls)mask, n)) continue;   // If neighbor coordinate does not exist, skip

                // If proposed next cell is the closest proposed unvisted cell yet, remember it
            if (get_distance(curr_position.x, curr_position.y, n.x, n.y) < shortestUnvisitedDistance && !maze[n.x][n.y].visited){
                shortestUnvisitedDistance = get_distance(curr_position.x, curr_position.y, n.x, n.y);   // Save cartesian distance
                bX = n.x;                                                                               // Save best unvisted x-coord
                bY = n.y;                                                                               // Save best unvisted y-coord
                dir = (walls)mask;                                                                      // Save relative unvistited next cell direction

                // Else, if the proposed next cell is the closest cell that has already been visited, remember it
            }else if (get_distance(curr_position.x, curr_position.y, n.x, n.y) < shortestVisitedDistance && maze[n.x][n.y].visited){
                shortestVisitedDistance = get_distance(curr_position.x, curr_position.y, n.x, n.y);      // Save teh cartesian distance
                bvX = n.x;                                                                               // Save the best visited x-coord
                bvY = n.y;                                                                               // Save teh best visited y-coord
                vdir = (walls)mask;                                                                      // Save relative visited next cell direction
            }

        }
        mask = mask << 1;                                                                                 // Shift mask to left


    }


    if (shortestUnvisitedDistance == 65535){                                    // If there is no new cell that has not been visited
        PlaySound(soundBlip);                                                   // Sound playing (for debug purposes)
        wait1Msec(250);
        PlaySound(soundBlip);

        curr_position.x = bvX;                                                  // Set the current position to the best visited cell x-coord
        curr_position.y = bvY;                                                  // Set the current position to the best visited cell y-coord
       
        maze[curr_position.x][curr_position.y].cell_walls |= (walls) opp_wall_lookup[(int) vdir];
                        // Close off the dead end by putting a virtual wall

        return vdir;                                                            // Return the visited direction

    }else{                                                                      // Else (there is a new cell to be travelled to)

        PlaySound(soundBlip);                                                   // Play sound for debug purposes

        curr_position.x = bX;                                                   // Set the current position to the best unvisited cell x-coord
        curr_position.y = bY;                                                   // Set the current position to the best unvisited cell y-coord
        return dir;                                                             // Return the new cell direction
    }
}

/********************************************************************************
 * Function: get_distance
 * Parameters: Takes integer values of (x1,y1) and (x2, y2)
 * Return: Distance as a float
 * Description: Function that determines the cartesian distance between two points
 */
float get_distance(int x1, int y1, int x2, int y2){
    return sqrt( (x2 - x1)* (x2 - x1) + (y2 - y1)* (y2 - y1));

}

/********************************************************************************
 * Function: get_neighbor_coordinate
 * Parameters: Takes the x,y corrdinate of current position, the proposed direction and a coord object to save the coordinate into
 * Return: True or false depending on whether proposed cell is valid or not
 * Description: Function determines which direction that the robot should move in. If there is a best
 *              route that facilitates an unvisited adjacent cell, the robot will travel there, however,
 *              if there is no open cell that is unvisited, the robot will go back to the cell that it had come
 *              from.
 */
bool get_neighbor_coordinate(int x, int y, walls dir, coord *n){

    // Figure out the coordinate based on the current position and the desired direction
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

        if (n->x < 0 || n->y < 0 || n->x >= MAZE_WIDTH  || n->y >= MAZE_HEIGHT) return false;   // if the cell is not valid, return false
        return true;                                                                            // else return true as it is valid


}
