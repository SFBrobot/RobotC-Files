#pragma config(Sensor, in1,    liftPot,        sensorPotentiometer)
#pragma config(Sensor, in2,    chainPot,       sensorPotentiometer)
#pragma config(Sensor, in3,    clawPot,        sensorPotentiometer)
#pragma config(Sensor, dgtl1,  lDriveEnc,      sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  rDriveEnc,      sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  goalLim,        sensorTouch)
#pragma config(Sensor, dgtl6,  lJump,          sensorDigitalIn)
#pragma config(Sensor, dgtl7,  rJump,          sensorDigitalIn)
#pragma config(Motor,  port1,           blDrive,       tmotorVex393HighSpeed_HBridge, openLoop, driveLeft)
#pragma config(Motor,  port2,           tlDrive,       tmotorVex393HighSpeed_MC29, openLoop, driveLeft, encoderPort, dgtl1)
#pragma config(Motor,  port3,           lGoalMtr,      tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port4,           clawMtr,       tmotorVex393HighSpeed_MC29, openLoop, driveLeft)
#pragma config(Motor,  port5,           lLiftMtr,      tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port6,           rLiftMtr,      tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           chainMtr,      tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port8,           rGoalMtr,      tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           trDrive,       tmotorVex393HighSpeed_MC29, openLoop, reversed, driveRight, encoderPort, dgtl3)
#pragma config(Motor,  port10,          brDrive,       tmotorVex393HighSpeed_HBridge, openLoop, reversed, driveRight)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

#include "srBot1v1 Autons - Unstable.h"
#include "pid.h"

/*

Motor Configuration

Port 1 - Bottom Left Drive Motor
Port 2 - Top Left Drive Motor
Port 3 - Left Mobile Goal Lift Motor (Power Expander)
Port 4 - Cone Intake Motor (Claw)
Port 5 - Left 4-Bar Lift Motor (Power Expander)
Port 6 - Right 4-Bar Lift Motor (Power Expander)
Port 7 - Chain Bar Lift Motor (Y-Cable)
Port 8 - Right Mobile Goal Lift Motor (Power Expander)
Port 9 - Top Right Drive Motor
Port 10- Bottom Right Drive Motor

*/

/*

Digital Sensor Configuration


Ports 1 & 2 - Left Drive Encoder

Ports 3 & 4 - Right Drive Encoder

Port 5 - Mobile Goal Intake Limit Switch
Port 6- Left Tile Autonomous Jumper Clip
Port 7- Right Tile Autonomous Jumper Clip

*/

/*

Analog Sensor Configuration


Port 1 - 4-Bar Lift Potentiometer

Port 2 - Chain Bar Potentiometer

Port 3 - Claw Potentiometer

*/



//Macro Definitions

#ifndef MACRO_DEF //Include guard; if macros are already defined, don't define them again

#define MACRO_DEF

//Setting Left Drive Motor

#define setDriveL(pwr) motor[blDrive] = \
	motor[tlDrive] = \
	pwr


//Setting Right Drive Power

#define setDriveR(pwr) motor[brDrive] = \
	motor[trDrive] = \
	pwr


//Setting Mobile Goal Lift Power

#define setGoal(pwr) motor[lGoalMtr] = \
	motor[rGoalMtr] = \
	pwr


//Set Cone Claw Power

#define setClaw(pwr) motor[clawMtr] = pwr


//Set Chain Bar Power

#define setChain(pwr) motor[chainMtr] = pwr


//Set 4-Bar Lift Power

#define setLift(pwr) motor[lLiftMtr] = \
	motor[rLiftMtr] = \
	pwr

#endif


//Global Variables

Pid lDrivePid,
	rDrivePid,
	liftPid,
	clawPid,
	goalPid;

Pid* pPids[5]; //Array of pointers to Pid structs

float lDrivePidConsts[3], //Arrays containing PID constants
	rDrivePidConsts[3],
	liftPidConsts[3],
	clawPidConsts[3],
	goalPidConsts[3];




//Function Definitions



//Arcade Drive Control Scheme

void arcade(short* ins) { //Takes pointer to short array of input values
	setDriveL(ins[2] + ins[0]); //Left power is equal to sum of x and y axes
	setDriveR(ins[2] - ins[0]); //Right power is equal to difference of y and x axes
}


//Arcade Drive Control Scheme

void arcade(short x, short y) { //Takes 2 short arguments
	setDriveL(y + x); //Left power is equal to sum of x and y axes
	setDriveR(y - x); //Right power is equal to difference of y and x axes
}





void pre_auton() {

	//Init PIDs
	pPids[0] = initPid(&lDrivePid, lDrivePidConsts);
	pPids[1] = initPid(&rDrivePid, rDrivePidConsts);
	pPids[2] = initPid(&liftPid, liftPidConsts);
	pPids[3] = initPid(&clawPid, clawPidConsts);
	pPids[4] = initPid(&goalPid, goalPidConsts);

	bLCDBacklight = true; //Enable LCD backlight to make sure reading

  bStopTasksBetweenModes = true;

}




task autonomous() {
	if(SensorValue[lJump] ^ SensorValue[rJump]) {
		if(SensorValue[lJump])
			leftAuton();
		else
			rightAuton();
	}
	else
		defaultAuton();
}



task usercontrol() {
	// User control code here, inside the loop

	short sticks[4]; //Stores threshold-adjusted joystick values

	while (true) {

		//Implement threshold comparison to eliminate motor whine caused by near-zero motor power
		for(int i = 0; i < 4; i++) { //Iterate over all 4 joystick axes and all 4 members of sticks
			if(fabs(vexRT[i]) >= 7) //If stick value is outside deadband
				sticks[i] = vexRT[i]; //Store axis value in associated member of sticks
			else
				sticks[i] = 0; //Otherwise set associated sticks member to 0
		}

		arcade(sticks); //Pass address of sticks as argument to arcade() function declared on line 120

		if(vexRT[Btn5U]) //If 5 up is pressed
			setLift(127); //Run lift full power up
		else if(vexRT[Btn5D]) //If 5 up is not pressed but 5 down is
			setLift(-127); //Run lift full power down
		else //If neither 5 up or 5 down is pressed
			setLift(0); //Stop DR4B

		if(vexRT[Btn6U]) //If 6 up is pressed
			setChain(127); //Run chain bar full power up
		else if(vexRT[Btn6D]) //If 6 up is not pressed but 6 down is
			setChain(-127); //Run chain bar full power down
		else //If neither 6 up or 6 down is pressed
			setChain(0); //Stop chain bar

		if(vexRT[Btn7U]) //If 7 up is pressed
			setClaw(127); //Run claw full power open
		else if(vexRT[Btn7D]) //If 7 up is not pressed but 7 down is
			setClaw(-127); //Run claw full power closed
		else //If neither 7 up or 7 down is pressed
			setClaw(0); //Stop claw

		if(vexRT[Btn8U]) //If 8 up is pressed
			setGoal(127); //Run mobile goal lift full power up
		else if(vexRT[Btn8D]) //If 8 up is not pressed but 8 down is
			setGoal(-127); //Run mobile goal lift full power down
		else //If neither 8 up or 8 down is pressed
			setGoal(0); //Stop goal lift

	}
}
