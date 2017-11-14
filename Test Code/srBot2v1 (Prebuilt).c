#pragma config(Sensor, in1,    liftPot,        sensorPotentiometer)
#pragma config(Sensor, in2,    goalPot,        sensorPotentiometer)
#pragma config(Sensor, dgtl1,  lDriveEnc,      sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  rDriveEnc,      sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  stackEnc,       sensorQuadEncoder)
#pragma config(Motor,  port1,           intake,        tmotorVex393HighSpeed_HBridge, openLoop)
#pragma config(Motor,  port2,           bblWheel,      tmotorVex393HighSpeed_MC29, openLoop, driveLeft)
#pragma config(Motor,  port3,           tblWheel,      tmotorVex393HighSpeed_MC29, openLoop, driveLeft)
#pragma config(Motor,  port4,           flWheel,       tmotorVex393HighSpeed_MC29, openLoop, driveLeft)
#pragma config(Motor,  port5,           lift,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           goal,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           frWheel,       tmotorVex393HighSpeed_MC29, openLoop, reversed, driveRight)
#pragma config(Motor,  port8,           tbrWheel,      tmotorVex393HighSpeed_MC29, openLoop, reversed, driveRight)
#pragma config(Motor,  port9,           bbrWheel,      tmotorVex393HighSpeed_MC29, openLoop, reversed, driveRight)
#pragma config(Motor,  port10,          stack,         tmotorVex393HighSpeed_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//


#include "pid.h"


/*
Systems
Drive - 6 Motors
DR4B - 2 Motors
Intake - 1 Motor
Mobile Goal Intake - 0 Motors
Mobile Goal Lift - 2 Motors
Cone Stacker - 1 Motor
*/

#define setDriveR(pwr) motor[bbrWheel] = \
	motor[tbrWheel] = \
	motor[frWheel] = \
	pwr

#define setDriveL(pwr) motor[bblWheel] = \
	motor[tblWheel] = \
	motor[flWheel] = \
	pwr

#define setLift(pwr) motor[lift] = pwr

#define setGoal(pwr) motor[goal] = pwr

#define setIntake(pwr) motor[intake] = \
	pwr

#define setStack(pwr) motor[stack] = \
	pwr


//STRUCTS
Pid pids[6];

//CONSTANTS
const float pidConsts[6][3] = {
	{ 0.1, 0.001, 0.01 },
	{ 0.1, 0.001, 0.01 },
	{ 0.1, 0.001, 0.01 },
	{ 0.1, 0.001, 0.01 },
	{ 0.1, 0.001, 0.01 },
	{ 0.1, 0.001, 0.01 }
};
const char STICK_THRESH = 11;

enum System { LEFT_DRIVE = 0, RIGHT_DRIVE = 1, LIFT = 2, STACK = 3, INTAKE = 4, GOAL = 5 };

void setAllMtrPwrs(int* mtrPwrs) {
	setDriveL(mtrPwrs[LEFT_DRIVE]);
	setDriveR(mtrPwrs[RIGHT_DRIVE]);
	setLift(mtrPwrs[LIFT]);
	setStack(mtrPwrs[STACK]);
	setIntake(mtrPwrs[INTAKE]);
	setGoal(mtrPwrs[GOAL]);
}

task main() {

	short sticks[4];
	int mtrPwrs[6],
		pwrsLast[6] = { 0, 0, 0, 0, 0, 0 };
/*
0 - left drive
1 - right drive
2 - lift
3 - stack
4 - intake
5 - goal
*/

	for(int i = 0; i < (sizeof(pids) / sizeof(Pid)); i++)
		initPid(&pids[i], &pidConsts[i][0]);

	while(true) {
		for(int i = 0; i < sizeof(sticks) / sizeof(short); i++) {

			if(fabs(vexRT[i]) > STICK_THRESH)
				sticks[i] = vexRT[i];
			else
				sticks[i] = 0;

		}

		mtrPwrs[LEFT_DRIVE] = (sticks[2] + sticks[3]);
		mtrPwrs[RIGHT_DRIVE] = (sticks[2] - sticks[3]);

		if(vexRT[Btn5U] ^ vexRT[Btn5D]) {
			if(vexRT[Btn5U]) {
				mtrPwrs[LIFT] = 127;
			}
			else {
				mtrPwrs[LIFT] = -127;
			}
		}
		else {
			mtrPwrs[LIFT] = upPid(&pids[LIFT], SensorValue[liftPot]);
		}

		if(vexRT[Btn6U] ^ vexRT[Btn6D]) {
			if(vexRT[Btn6U]) {
				mtrPwrs[STACK] = 127;
			}
			else {
				mtrPwrs[STACK] = -127;
			}
		}
		else {
			mtrPwrs[STACK] = 0;
		}

		if(vexRT[Btn8U] ^ vexRT[Btn8D]) {
			if(vexRT[Btn8U]) {
				mtrPwrs[GOAL] = 127;
			}
			else {
				mtrPwrs[GOAL] = -127;
			}
		}
		else {
			mtrPwrs[GOAL] = 0;
		}

		if(vexRT[Btn8R] ^ vexRT[Btn8L]) {
			if(vexRT[Btn8R]) {
				mtrPwrs[INTAKE] = 127;
			}
			else {
				mtrPwrs[INTAKE] = -127;
			}
		}
		else {
			mtrPwrs[INTAKE] = 0;
		}

		setAllMtrPwrs(mtrPwrs);

	}
}
