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

int mtrPort = 0;
string motorNames[10] = { "roller", "rLift", "lLift", "frDrive", "brDrive", "ylDrive", "mrDrive", "mlDrive", "lGoal", "rGoal" };

task main() {
	word btnLast = (word)false;
	while(true) {
		if(mtrPort > 9) {
			mtrPort = 0;
			motor[9] = 0;
		}

		motor[mtrPort] = 127;
		if(mtrPort > 0)
			motor[mtrPort-1] = 0;

		if(vexRT[Btn5U] && !btnLast) {
			mtrPort++;
			writeDebugStreamLine("Motor Port: %d", mtrPort);
			writeDebugStreamLine("Motor: %s", motorNames[mtrPort]);
		}

		btnLast = vexRT[Btn5U];

		wait1Msec(20);

	}
}
