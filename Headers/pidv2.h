#ifndef PID_H

#define PID_H

int max(int a, int b) {
	return (a > b)
		? a
		: b;
}

int min(int a, int b) {
	return (a < b)
		? a
		: b;
}

typedef struct {

	int targ,
		val,
		err,
		errLast,
		pwr,
		thresh,
		time,
		timeLast,
		dt;

	float kP,
		kI,
		kD,
		prop,
		integ,
		deriv,
		result;

	bool bIsOnTarg,
		bIsEnabled;

} Pid;

Pid* initPid(Pid* pid, float kP, float kI, float kD, int thresh) {
	pid->kP = kP;
	pid->kI = kI;
	pid->kD = kD;
	pid->thresh = thresh;
	pid->targ = 0;
	pid->integ = 0;
	pid->timeLast = 0;
	pid->errLast = 0;

	return pid;
}



void initPids(Pid* pids, float* kPs, float* kIs, float* kDs, int* threshes, int pidCt) {
	for(int i = 0; i < pidCt; i++) {
		initPid((pids+i), kPs[i], kIs[i], kDs[i], threshes[i]);
	}
}

int setTarg(Pid* pid, int targ) {
	pid->targ = targ;
	pid->integ = 0;
	pid->errLast = 0;
	return pid->targ;
}

int calcPid(Pid* pid, int val, int time) {
	pid->val = val;
	pid->err = pid->targ - pid->val;
	pid->time = time;
	pid->dt = pid->time - pid->timeLast;
	if(fabs(pid->err) < pid->thresh) {
		pid->prop = 0;
		pid->integ = 0;
		pid->deriv = 0;
		pid->result = 0;
	}
	else {
		pid->prop = pid->err * pid->kP;
		pid->integ += (pid->err * pid->kI) * pid->dt;
		pid->deriv = (pid->err - pid->errLast) * pid->kD / pid->dt;
		pid->result = pid->prop + pid->integ + pid->deriv;
		pid->result = max(min(pid->result, 127), -127);
	}
	pid->errLast = pid->err;
	pid->timeLast = pid->time;

	return pid->result;
}

int calcPidPwr(Pid* pid) {
	pid->pwr = round(pid->result);
	pid->pwr = max(min(pid->pwr, 127), -127);
	return pid->pwr;
}

int calcPidPwr(Pid* pid, int val, int time) {
	pid->pwr = round(calcPid(pid, val, time));
	pid->pwr = max(min(pid->pwr, 127), -127);
	return pid->pwr;
}

#endif
