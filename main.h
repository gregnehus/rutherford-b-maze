#pragma config(Sensor, S1,     touchSensor,         sensorTouch)
#pragma config(Sensor, S4,     sonarSensor,         sensorSONAR)
#pragma config(Motor,  motorA,          rightMotor,    tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  motorB,          turretMotor,    tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  motorC,          leftMotor,     tmotorNormal, PIDControl, encoder)
#define MAZE_HEIGHT 5
#define MAZE_WIDTH 6



typedef enum {
  west = 1,   // 0001
  north = 2,  // 0010
  east = 4,   // 0100
  south = 8   // 1000
} walls;

typedef struct {
  walls cell_walls;
  char visited;
} cell;

typedef struct{
  int x;
  int y;

  int angle;
} position;

position curr_position;
cell maze[MAZE_WIDTH][MAZE_HEIGHT];
