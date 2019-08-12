#include "Enemy.h"
#include <random>
#include <ctime>

// Constructs empty enemy => defaults as floor tile
Enemy::Enemy() : LevelObject() {
	// In the event that enemy randomly placed is not assigned => place floor tile instead
	_tile = '.';
}

// Parameterised constructor
Enemy::Enemy(string name, char tile, int level, int attack, int defense, int health, int experienceValue) {
	_name = name;
	_tile = tile;
	_level = level;
	_attack = attack;
	_defense = defense;
	_health = health;
	_experienceValue = experienceValue;
}

// Copy assignment, overloading +=
Enemy & Enemy::operator+=(Enemy &enemy) {
	_name = enemy._name;
	_tile = enemy._tile;
	_level = enemy._level;
	_attack = enemy._attack;
	_defense = enemy._defense;
	_health = enemy._health;
	_experienceValue = enemy._experienceValue;
	return *this;
}


//Sets position of enemy
void Enemy::setPosition(int x, int y) {
	_x = x;
	_y = y;
}

//Get position of enemy using reference variables
void const Enemy::getPosition(int &x, int &y) {
	x = _x;
	y = _y;
}

string Enemy::getName() {
	return _name;
}
char Enemy::getTile() {
	return _tile;
}

int Enemy::attack() {
	int TIME = (int)(time(NULL));
	static default_random_engine randomEngine(TIME);
	uniform_int_distribution<int> attackChance(0, _attack); // STL template

	return attackChance(randomEngine);
}

int Enemy::takeDamage(int attack) {
	attack -= _defense;
	//Check if attack did damage
	if (attack > 0) {
		_health -= attack;
		if (_health <= 0) {
			return _experienceValue;
		}
		else {
			return 0; // enemy still alive
		}
	}
	else {
		return 0; // no damage dealt and enemy still alive
	}
}

char Enemy::getMove(int playerX, int playerY) {
	int TIME = (int)time(NULL);
	static default_random_engine randomEngine(TIME);
	uniform_int_distribution<int> moveChance(0, 6);

	int distance; //distance from player
	int dx = _x - playerX;
	int dy = _y - playerY;
	int adx = abs(dx);
	int ady = abs(dy);

	distance = adx + ady;

	const int chaseDistance = 5; // 5 is the chase range

	if (distance <= chaseDistance) {
		//Move along X axis
		if (adx > ady) {
			if (dx > 0) {
				return 'a';
			}
			else {
				return 'd';
			}
		}
		else { //Move along Y axis
			if (dy > 0) {
				return 'w';
			}
			else {
				return 's';
			}
		}

	}

	int randomMove = moveChance(randomEngine);
	switch (randomMove) {
	case 0:
		return 'a';
	case 1:
		return 'w';
	case 2:
		return 's';
	case 3:
		return 'd';
	default:
		return '.'; //Don't move
	}

}