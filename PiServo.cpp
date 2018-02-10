#include "PiServo.hpp"

using namespace std;
using namespace std::chrono_literals;

FLOAT sleepTimeMS = 10.f;
INT32 Pos[2] = { 0, 0 };

INT32 GetIntInput() {
	try {
		string in;
		getline(cin, in);
		INT32 res = stoi(in);
		return res;
	} catch (exception& e) {
		cout << e.what() << endl;
		return GetIntInput();
	}
}

FLOAT GetFloatInput() {
	try {
		string in;
		getline(cin, in);
		FLOAT res = stof(in);
		return res;
	} catch (exception& e) {
		cout << e.what() << endl;
		return GetFloatInput();
	}
}

void Step(Motor m, Direction d) {
	if (d == Left) {
		Pi::lStep(m);
		--Pos[m];
	} else {
		Pi::rStep(m);
		++Pos[m];
	}
}

void Move(INT32 x, INT32 y) {

	Direction hor = x > 0 ? Right : Left;
	Direction vert = y > 0 ? Right : Left;

	x = abs(x);
	y = abs(y);

	INT32 tx = x;
	INT32 ty = y;

	INT32 max = max(x, y);

	for (INT32 i = max - 1; i >= 0; --i) {

		FLOAT m = fdiv(i, max);
		FLOAT fx = fdiv(x, tx);
		FLOAT fy = fdiv(y, ty);

		if (fx >= m && x > 0) {
			Step(Horiz, hor);
			--x;
		}
		if (fy >= m && y > 0) {
			Step(Vertic, vert);
			--y;
		}

		sleep(sleepTimeMS);

	}

}

void MoveMilli(FLOAT x, FLOAT y) {
		Move(static_cast<INT32>(48 * x), static_cast<INT32>(48 * y));
}

void SetSleepTime() {
	cout << "Current SleepTime: " << sleepTimeMS << "ms\n > ";
	FLOAT input = GetFloatInput();
	if (input < 0) return;
	sleepTimeMS = input;
}

void MoveStepQuery() {
	cout << "Steps Horizontal: ";
	int x = GetIntInput();
	cout << "Steps Vertical: ";
	int y = GetIntInput();
	Move(x, y);
}

//using millimeters
void MoveMilliQuery() {
	cout << "Move Horizontal: ";
	FLOAT x = GetFloatInput();
	cout << "Move Vertical: ";
	FLOAT y = GetFloatInput();
	MoveMilli(x, y);
}

void MoveHorizMilliQuery() {
	cout << "Move Horizontal: ";
	FLOAT x = GetFloatInput();
	Move(INT32(48 * x), 0);
}

void MoveVertMilliQuery() {
	cout << "Move Vertical: ";
	FLOAT y = GetFloatInput();
	Move(0, INT32(48 * y));
}

void Reset() {
	Move(-Pos[X], -Pos[Y]);
}

void Parabola() {
	constexpr INT32 resolution = 20;
	constexpr FLOAT width = 20/*mm*/; // 3cm
	for (INT32 i = 0; i < resolution; ++i) {
		constexpr FLOAT x = width / resolution;
		FLOAT y = pow(i / 10.f, 2);
		MoveMilli(x, y);
	}
}

INT32 GetProgramInput() {
	ClearConsole();
	cout << "--------------------\n";
	cout << " Current Position: " << Pos[X] << " | " << Pos[Y] << endl;
	cout << " 0: Set Sleeptime" << endl;
	cout << " 1: Move Steps" << endl;
	cout << " 2: Move Millimeter" << endl;
	cout << " 3: Parabola" << endl;
	cout << " 4: Move Horizontal Milli" << endl;
	cout << " 6: Move Vertical Milli" << endl;
	cout << " 5: Reset" << endl;
	cout << "--------------------\n > ";
	return GetIntInput();
}


int main() {

	Pi::Init();

	while (true) {

		INT32 input = GetProgramInput();

		switch (input) {
		case 0:
			SetSleepTime();
			break;
		case 1:
			MoveStepQuery();
			break;
		case 2:
			MoveMilliQuery();
			break;
		case 3:
			Parabola();
			break;
		case 4:
			MoveHorizMilliQuery();
			break;
		case 5:
			Reset();
			break;
		case 6:
			MoveVertMilliQuery();
			break;
		}

	}

}