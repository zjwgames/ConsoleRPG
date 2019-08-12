#include "Environmental.h"

Environmental::Environmental(string name) : LevelObject(name) {
}

Environmental::~Environmental() {}

// Sets door object name
void Environmental::setName(string name) {
	_name = name;
}

// Sets position of door
void Environmental::setPosition(int x, int y) {
	_x = x;
	_y = y;
}

// Gets level object name
string Environmental::getName() {
	return _name;
}

// Get position of player using reference variables
void const Environmental::getPosition(int &x, int &y) {
	x = _x;
	y = _y;
}

// --- DOOR ---

// Constructor
Door::Door(string name, bool locked) : Environmental(name) { //Use base class constructor for naming
	_locked = locked;
}

// Destructor
Door::~Door() {}

// --- SIGN ---

// Constructor
Sign::Sign(string name, string message) : Environmental(name) {
	_message = message;
}

// Destructor
Sign::~Sign() {}

string Sign::getMessage() {
	return _message;
}