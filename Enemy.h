#ifndef ENEMY_H
#define ENEMY_H

#pragma once

#include "LevelObject.h"
#include <string>

using namespace std;

class Enemy : public LevelObject {
public:

	// Default constructor - for general enemy, overwritten by += overload
	// => defaults to floor tile for placement in level generation
	Enemy();

	// Parameterised constructor - for Enemy with given properties
	Enemy(string name, char tile, int level, int attack, int defense, int health, int experienceValue);

	// Copy assignment overload +=
	// => allows general enemy to be overwritten by += operator
	// => used in level generation for choosing random enemy types
	Enemy & operator+=(Enemy &enemy);

	//Setters
	void setPosition(int x, int y); //sets enemy position in level

	//Getters
	void const getPosition(int &x, int &y); //gets enemy position in level
	string getName(); //gets enemy name
	char getTile(); //gets enemy tile (i.e. enemy representation in level)

	int attack(); //processes enemy attack on player, returns attackChance
	int takeDamage(int attack); // returns 0 if the enemy still alive, else returns XP reward for player

	//gets AI move command
	// => enemy moves in random direction, generally
	// => if enemy is within 5 tiles of player, then enemy chases player
	char getMove(int playerX, int playerY);

private:
	string _name; //stores enemy name
	char _tile; //stores enemy tile (i.e. enemy representation in level)

	int _x; //stores enemy x-coord in level
	int _y; //stores enemy y-coord in level

	int _level; //stores enemy level
	int _attack; //stores enemy attack
	int _defense; //stores enemy defense
	int _health; //stores enemy health
	int _experienceValue; // stores XP reward (i.e. how much experience the player gets for defeating enemy)

	//vector <Item> possibleDrops; // include items enemy can drop

};

#endif