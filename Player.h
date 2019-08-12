#ifndef PLAYER_H
#define PLAYER_H

#pragma once

#include "LevelObject.h"
#include "Item.h"
#include <vector>

class Player : public LevelObject {
public:
	Player();

	//Initialises player with starting player statistics
	void init(int level, int health, int attack, int defense, int experience);

	//Setters
	void setPosition(int x, int y); //sets player position

	//Getters
	void const getPosition(int &x, int &y); //get player position

	// Stats getters - used for player stats display in Level::print
	int const getLevel(); //gets player level
	int const getHealth(); //gets player health
	int const getAttack(); //gets player attack
	int const getDefense(); //gets player defense
	int const getExperience(); //gets player experience

	// Inventory getters
	vector <string> getInventoryList(); //gets inventory list, returns _inventory vector
	unsigned int getInventorySize(); //gets current size of _inventory vector
	unsigned int getInventoryMax(); //gets max allowed size of _inventory vector

	Item getItem(int index); //gets item of a given index from _inventory vector

	bool hasKey(); //returns whether or not player has a key in inventory

	int attack(); //processes player attack on enemy, returns attackChance
	int takeDamage(int attack); //processes player damage, returns 1 if player died, 0 if not
	bool addExperience(int experience); //adds experience to player, returns 1 if player levelled up, 0 if not
	void addItem(Item &item); //adds item to player _inventory vector
	void useItem(int inventorySlot); //uses item of given inventorySlot on player
private:
	//Properties
	int _level; //stores player level
	int _health; //stores player health
	int _attack; //stores player attack
	int _defense; //stores player defense
	int _experience; //stores player experience
	int _x; //stores player x-coord in level
	int _y; //stores player y-coord in level
	vector <Item> _inventory; // _inventory vector contains items player has picked up in level
	const unsigned int _inventoryMax = 8; //stores max allowed size of player inventory
};

#endif