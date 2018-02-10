#include <wiringPi.h>
#include <wiringPiI2C.h>

#include <iostream>
#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono_literals;

const float PI = 3.1415926f;
const int PIN0 = 0x6;
const int servos = 2;

using UINT32 = unsigned int;
using INT32 = int;
using FLOAT = float;

int stepst[servos] = { 0, 0 }; //step state

#ifdef max 
#undef max
#endif

#ifdef min
#undef min
#endif

#define max(a,b) (a > b) ? a : b
#define min(a,b) (a < b) ? a : b

void inline sleep(FLOAT ms) {
	this_thread::sleep_for(chrono::duration<float, milli>(ms));
}

FLOAT inline fdiv(INT32 nomin, INT32 denom) {
	return static_cast<FLOAT>(nomin) / static_cast<FLOAT>(denom);
}

void ClearConsole() {
#ifdef __linux__
	system("clear");
#else
	system("cls");
#endif
}

typedef enum { Horiz, Vertic, X = 0, Y = 1 } Motor;
typedef enum { Left, Right } Direction;

namespace Pi {

	inline void Init() {
		wiringPiI2CSetup(0x41);
		wiringPiI2CWriteReg8(3, 0x1, 0x4);
		wiringPiI2CWriteReg8(3, 0x0, 0x1);
	}

	inline void SetPwm(int ch, int up, int down) {
		int choff = 4 * ch;
		wiringPiI2CWriteReg8(3, PIN0 + choff + 0, up & 0xFF);
		wiringPiI2CWriteReg8(3, PIN0 + choff + 1, (up >> 8) & 0xF);
		wiringPiI2CWriteReg8(3, PIN0 + choff + 2, down & 0xFF);
		wiringPiI2CWriteReg8(3, PIN0 + choff + 3, (down >> 8) & 0xF);
	}

	inline void Set(int ch, int val) {
		if (val) SetPwm(ch, 0, 4095);
		else SetPwm(ch, 0, 0);
	}

	inline void Set(int servo, int a, int b, int c, int d) {
		Set(4 * servo + 0, a);
		Set(4 * servo + 1, b);
		Set(4 * servo + 2, c);
		Set(4 * servo + 3, d);
	}

	inline void rStep(int servo) {
		switch (stepst[servo]) {
		case 0:
			Set(servo, 1, 0, 1, 0);
			stepst[servo] = 1;
			break;
		case 1:
			Set(servo, 0, 1, 1, 0);
			stepst[servo] = 2;
			break;
		case 2:
			Set(servo, 0, 1, 0, 1);
			stepst[servo] = 3;
			break;
		case 3:
			Set(servo, 1, 0, 0, 1);
			stepst[servo] = 0;
			break;
		}
	}

	inline void lStep(int servo) {
		switch (stepst[servo]) {
		case 2:
			Set(servo, 1, 0, 1, 0);
			stepst[servo] = 1;
			break;
		case 3:
			Set(servo, 0, 1, 1, 0);
			stepst[servo] = 2;
			break;
		case 0:
			Set(servo, 0, 1, 0, 1);
			stepst[servo] = 3;
			break;
		case 1:
			Set(servo, 1, 0, 0, 1);
			stepst[servo] = 0;
			break;
		}
	}

	void Release() {
		wiringPiI2CWriteReg8(3, 0xFA, 0);
		wiringPiI2CWriteReg8(3, 0xFB, 0);
		wiringPiI2CWriteReg8(3, 0xFC, 0);
		wiringPiI2CWriteReg8(3, 0xFD, 0);
	}

}