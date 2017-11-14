
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

#ifndef FUNCS

#define FUNCS

int arcadeL(short* ins) {
	return ins[2] + ins[3];
}

int arcadeR(short* ins) {
	return ins[2] - ins[3];
}

void arcade(short* ins) { //Takes pointer to short array of input values
	setDriveL(arcadeL(ins)); //Left power is equal to sum of x and y axes
	setDriveR(arcadeR(ins)); //Right power is equal to difference of y and x axes
}

void arcade(short x, short y) { //Takes 2 short arguments
	setDriveL(y + x); //Left power is equal to sum of x and y axes
	setDriveR(y - x); //Right power is equal to difference of y and x axes
}

#endif
