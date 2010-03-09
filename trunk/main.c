#pragma config(Sensor, S1,     touchSensor,         sensorTouch)
#pragma config(Sensor, S4,     sonarSensor,         sensorSONAR)
#pragma config(Motor,  motorA,          rightMotor,    tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  motorB,          turretMotor,    tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  motorC,          leftMotor,     tmotorNormal, PIDControl, encoder)

/* INCLUDES */
#include "rbmaze.h"

/* DEFINES */
#define DURATION_TURN_90 400
#define DURATION_LOOK_90 400

/**/
typedef struct {
  int x;
  int y;
} coord;

typedef struct {
  char west;
  char north;
  char east;
  coord c;
} cell;

void halt()
{
	motor[rightMotor] = 0;
	motor[leftMotor] = 0;
	motor[turretMotor] = 0;
	wait1Msec(250);
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
		sonarValue = SensorValue(sonarSensor);                // Store Sonar Sensor values in 'sonarValue' variable.
		nxtDisplayCenteredTextLine(0, "Sonar Reading");       /* Display Sonar Sensor values */
		nxtDisplayCenteredBigTextLine(2, "%d", sonarValue);   /* to LCD screen using %d.     */
		wait1Msec(500);
  }
}

void scan_cell()
{
  wait1Msec(250);
  look_left(100+DURATION_LOOK_90);
  print_sonar();
  look_right(100+3*DURATION_LOOK_90);
  print_sonar();
  look_left(DURATION_LOOK_90);
  print_sonar();
  look_left(DURATION_LOOK_90);
  print_sonar();
  wait1Msec(250);
  return;
}

int choose_best_cell()
{
  return 0;
}



task main()
{
  eraseDisplay();
  nxtDisplayBigStringAt(0, 15, ":D  :D  :D");

  while(1){
    halt();
    scan_cell();
    halt();
    halt();
  }
  /*
  halt();
  scan_cell();
  dash(1250);
  scan_cell();
  turn_right(DURATION_TURN_90);
  dash(1250);
  scan_cell();
  turn_right(DURATION_TURN_90);
  dash(1250);
  scan_cell();
  turn_right(DURATION_TURN_90);
  dash(1250);
  scan_cell();
  turn_right(DURATION_TURN_90);
  halt();
  */
}
