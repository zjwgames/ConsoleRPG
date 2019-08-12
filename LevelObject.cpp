#include "LevelObject.h"

LevelObject::LevelObject() {
	_x = 0;
	_y = 0;
}

LevelObject::LevelObject(string name) {
	_name = name;
	_x = 0;
	_y = 0;
}

//Sets door object name
void LevelObject::setName(string name) {
	_name = name;
}

//Set position of level object
void LevelObject::setPosition(int x, int y) {
	_x = x;
	_y = y;
}

//Gets level object name
string LevelObject::getName() {
	return _name;
}

//Get position of level object using reference variables
void const LevelObject::getPosition(int &x, int &y) {
	x = _x;
	y = _y;
}
