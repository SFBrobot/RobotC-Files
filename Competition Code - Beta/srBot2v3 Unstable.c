#pragma config(UART_Usage, UART1, uartVEXLCD, baudRate19200, IOPins, None, None)
#pragma config(UART_Usage, UART2, uartNotUsed, baudRate4800, IOPins, None, None)
#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    liftPot,        sensorNone)
#pragma config(Sensor, in2,    goalPot,        sensorPotentiometer)
#pragma config(Sensor, dgtl1,  lDriveEnc,      sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  rDriveEnc,      sensorQuadEncoder)
#pragma config(Motor,  port1,           roller,        tmotorVex393HighSpeed_HBridge, openLoop)
#pragma config(Motor,  port2,           rLift,         tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port3,           lLift,         tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           frDrive,       tmotorVex393TurboSpeed_MC29, openLoop, reversed, driveRight)
#pragma config(Motor,  port5,           brDrive,       tmotorVex393TurboSpeed_MC29, openLoop, reversed, driveRight)
#pragma config(Motor,  port6,           ylDrive,       tmotorVex393TurboSpeed_MC29, openLoop, driveLeft)
#pragma config(Motor,  port7,           mrDrive,       tmotorVex393TurboSpeed_MC29, openLoop, driveRight)
#pragma config(Motor,  port8,           mlDrive,       tmotorVex393TurboSpeed_MC29, openLoop, reversed, driveLeft)
#pragma config(Motor,  port9,           lGoal,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          rGoal,         tmotorVex393_HBridge, openLoop)

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

#include "pidv2.h"


//CONSTANTS
int STICK_THRESH = 11;

//CONST ARRAYS
float[4] kPs = { 0.1, 0.1, 0.1, 0.1 };
float[4] kIs = { 0.001, 0.001, 0.001, 0.001 };
float[4] kDs = { 0.01, 0.01, 0.01, 0.01 };
int[4] thresh = { 60, 60, 100, 50 };

//STRUCTS
//PID
Pid[4] Pids;


//MACROS
#define setDriveL(pwr) motor[ylDrive] = \
	motor[mlDrive] = \
	pwr
#define setDriveR(pwr) motor[frDrive] = \
	motor[mrDrive] = \
	motor[brDrive] = \
	pwr
#define setLift(pwr) motor[rLift] = \
	motor[lLift] = \
	pwr
#define setGoal(pwr) motor[rGoal] = \
	motor[lGoal] = \
	pwr
#define setIntake(pwr) motor[roller] = pwr


/////////////////////////////////////////////////////////////////////////////////////////
//
//                          Pre-Autonomous Functions
//
// You may want to perform some actions before the competition starts. Do them in the
// following function.
//
/////////////////////////////////////////////////////////////////////////////////////////

void pre_auton()
{
	initPids(Pids, kPs, kIs, kDs, thresh);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 Autonomous Task
//
// This task is used to control your robot during the autonomous phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task autonomous()
{
	
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task usercontrol() {
	int sticks[4];
	// User control code here, inside the loop

	while (true) {
	  for(int i = 0; i < 4; i++) { //Iterate over all 4 joystick analog channels
	  	if(fabs(vexRT[i]) >= STICK_THRESH)
			sticks[i] = vexRT[i];
		else
			sticks[i] = 0;
		}

		//sticks[2] = vexRT[Ch3] (Left y-axis)
		//sticks[3] = vexRT[Ch4] (Left x-axis)
		setDriveL(sticks[2] + sticks[3]);
		setDriveR(sticks[2] - sticks[3]);

		if(vexRT[Btn5U] ^ vexRT[Btn5D]) {
			if(vexRT[Btn5U])
				motor[lLift] =
					motor[rLift] =
					127;
			else
				motor[lLift] =
					motor[rLift] =
					-127;
		}
		else
			motor[lLift] =
				motor[rLift] =
				0;

		if(vexRT[Btn6U] ^ vexRT[Btn6D]) {
			if(vexRT[Btn6U])
				motor[lGoal] =
					motor[rGoal] =
					127;
			else
				motor[lGoal] =
					motor[rGoal] =
					-127;
		}
		else
			motor[lGoal] =
				motor[rGoal] =
				0;

				if(vexRT[Btn8U] ^ vexRT[Btn8D]) {
			if(vexRT[Btn8U])
				motor[roller] =
					127;
			else
				motor[roller] =
					-127;
		}
		else
			motor[roller] =
				0;

		wait1Msec(20);
	}
}
