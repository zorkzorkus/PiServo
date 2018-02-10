#include <wiringPi.h>
#include <wiringPiI2C.h>

#define SPEED 10ms

const float PI = 3.1415926f;
const int PIN0 = 0x6;
const int servos = 2;

int stepst[servos] = { 0, 0 }; //step state

inline void init_servos() {
	wiringPiI2CWriteReg8(3, 0x1, 0x4);
	wiringPiI2CWriteReg8(3, 0x0, 0x1);
}

inline void setpwm(int ch, int up, int down) {
	int choff = 4 * ch;
	wiringPiI2CWriteReg8(3, PIN0 + choff + 0, up & 0xFF);
	wiringPiI2CWriteReg8(3, PIN0 + choff + 1, (up >> 8) & 0xF);
	wiringPiI2CWriteReg8(3, PIN0 + choff + 2, down & 0xFF);
	wiringPiI2CWriteReg8(3, PIN0 + choff + 3, (down >> 8) & 0xF);
}

inline void set(int ch, int val) {
	if (val) setpwm(ch, 0, 4095);
	else setpwm(ch, 0, 0);
}

inline void sets(int servo, int a, int b, int c, int d) {
	set(4 * servo + 0, a);
	set(4 * servo + 1, b);
	set(4 * servo + 2, c);
	set(4 * servo + 3, d);
}

inline void rstep(int servo) {
	switch (stepst[servo]) {
	case 0:
		sets(servo, 1, 0, 1, 0);
		stepst[servo] = 1;
		break;
	case 1:
		sets(servo, 0, 1, 1, 0);
		stepst[servo] = 2;
		break;
	case 2:
		sets(servo, 0, 1, 0, 1);
		stepst[servo] = 3;
		break;
	case 3:
		sets(servo, 1, 0, 0, 1);
		stepst[servo] = 0;
		break;
	}
}

inline void lstep(int servo) {
	switch (stepst[servo]) {
	case 2:
		sets(servo, 1, 0, 1, 0);
		stepst[servo] = 1;
		break;
	case 3:
		sets(servo, 0, 1, 1, 0);
		stepst[servo] = 2;
		break;
	case 0:
		sets(servo, 0, 1, 0, 1);
		stepst[servo] = 3;
		break;
	case 1:
		sets(servo, 1, 0, 0, 1);
		stepst[servo] = 0;
		break;
	}
}

void release() {
	wiringPiI2CWriteReg8(3, 0xFA, 0);
	wiringPiI2CWriteReg8(3, 0xFB, 0);
	wiringPiI2CWriteReg8(3, 0xFC, 0);
	wiringPiI2CWriteReg8(3, 0xFD, 0);
}