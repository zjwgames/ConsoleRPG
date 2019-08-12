#include "Item.h"

Item::Item() { // Tile defaults to floor tile if unassigned
	_tile = '.';
}

Item::Item(string name, char tile, bool usable, itemEffect effect, int magnitude) : LevelObject(name) {
	_tile = tile;
	_usable = usable;
	_effect = effect;
	_magnitude = magnitude;
	_x = 0;
	_y = 0;
}

Item::~Item() {}

// Copy assignment, overloading +=
Item & Item::operator+=(Item &item) {
	_name = item._name;
	_tile = item._tile;
	_usable = item._usable;
	_effect = item._effect;
	_magnitude = item._magnitude;
	return *this;
}

// Sets position of item
void Item::setPosition(int x, int y) {
	_x = x;
	_y = y;
}

// Sets item name
void Item::setName(string name) {
	_name = name;
}

// Get position of player using reference variables
void const Item::getPosition(int &x, int &y) {
	x = _x;
	y = _y;
}

// Gets item name
string Item::getName() {
	return _name;
}

// Get item tile
char Item::getTile() {
	return _tile;
}

// Get if Item is usable
bool Item::isUsable() {
	return _usable;
}

// Get Item effect
Item::itemEffect Item::getEffect() {
	return _effect;
}

// Get magnitude
int Item::getMagnitude() {
	return _magnitude;
}