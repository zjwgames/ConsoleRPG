#ifndef ITEM_H
#define ITEM_H

#pragma once
#include "LevelObject.h"

class Item : public LevelObject {
public:
	//enumerates types of item effects on player
	enum itemEffect {
		health,
		attack,
		defense,
		experience,
		none
	};
public:
	// Default constructor - for general item, overwritten by += overload
	// => defaults to floor tile for placement in level generation
	Item();

	// Parameterised constructor - for Item with given properties
	Item(string name, char tile, bool usable, itemEffect effect, int magnitude);
	// Destructor
	~Item();

	// Copy assignment overload +=
	// => allows general item to be overwritten by += operator
	// => used in level generation for choosing random item types
	Item & operator+=(Item &item);

	//Setters
	void setPosition(int x, int y); //sets item position in level
	void setName(string name); //sets item name

	// Getters
	void const getPosition(int &x, int &y); //gets item position in level
	string getName(); //gets item name
	char getTile(); //gets item tile (i.e. item representation in level)
	bool isUsable(); //gets if item is usable on player, returns 1 if usable, 0 if not
	itemEffect getEffect(); //gets item effect, returning enum of item effect type
	int getMagnitude(); //gets magnitude of item effect on player, 0 if item has no effect


protected:
	char _tile; //stores item tile (i.e. item representation in level)
	bool _usable; //stores if item is usable, 1 if usable, 0 if not
	itemEffect _effect; //stores type of item effect on player
	int _magnitude; //stores magnitude of item effect, 0 if item has no effect
};

#endif