#ifndef LEVEL_H
#define LEVEL_H

#pragma once

#include <vector>
#include <string>
#include <list>
#include <Windows.h>

#include "Player.h"
#include "LevelObject.h"
#include "Environmental.h"
#include "Item.h"
#include "Enemy.h"

using namespace std;

class Level {
private: //pre-declared variables
	enum enemy { //enumerates enemy types
		snake,
		goblin,
		bandit,
		ogre,
		dragon,
		MAX_ENEMIES
	};
	enum item { //enumerates item types
		healthPotion,
		attackPotion,
		defensePotion,
		book,
		key,
		MAX_ITEMS
	};
	unsigned int _currentLevel; //stores current level number
	static const unsigned int _levelSize = 32; //stores max level size
	bool _playerDied; //stores if player has died or not

public: //functions
	Level();

	void load(string fileName, Player &player); //loads level into levelData
	void saveAsStartRoom(); //saves current room as starting room for next run
	void print(Player &player); //prints level data, messages and player stats to screen

	void movePlayer(char input, Player &player); //moves player in level based in input
	void useItemOnPlayer(int inventorySlot, Player &player); //adds level item to player
	void updateEnemies(Player &player); //updates enemy positions and actions

	char getTile(int x, int y); //gets given x,y level tile from level data
	unsigned int getCurrentLevel(); //gets current level
	const unsigned int getLevelSize(); //gets max size of level
	bool getPlayerDied(); //returns if player has died in level

	void setPlayerDied(bool isDead); //sets _playerDied variable for level clean up

	void setTile(int x,  int y, char tile); //sets x,y level tile in level data to given tile

	void addMessage(const string message); // adds message to message list

private: //functions
	void loadNextLevel(Player &player, bool hiddenLevel); //clears old level data and loads next level
	void printLevel(vector <string> v); //prints level vector
	template <class T> //prints user-interface data - players stats and level messages
	void printUIData(HANDLE console, unsigned int startX, unsigned int startY, T data);
	void printUIInventory(HANDLE console, unsigned int startX, unsigned int startY, vector<string> data); //prints inventory
	void generate(Player &player); //generates new random level, saving as file

	//chooses random enemy for placement in level generation
	// => creates general enemy with default constructor
	// => then overwrites enemy with one of a random type (using += operator)
	Enemy chooseRandomEnemy();

	//chooses random item for placement in level generation
	// => creates general item with default constructor
	// => then overwrites item with one of a random type (using += operator)
	Item chooseRandomItem();

	void processPlayerMove(Player &player, int targetX, int targetY); //processes a player move, checks collisions
	void processEnemyMove(Player &player, int enemyIndex, int targetX, int targetY); //processes an enemy move, checks collisions
	void battleEnemy(Player &player, int targetX, int targetY); //handles enemy encounters, random rolls for player/enemy attack turns
	void pickupItem(Player &player, int targetX, int targetY); //checks item encountered for player to pickup

private: //variables
	vector <string> _levelStartData; //starting level config for saving
	vector <string> _levelData; //stores level config
	list <string> _levelMessages; //stores player/enemy/level action messages
	vector <Enemy> _enemies; //stores level enemies
	vector <Door> _doors; //stores doors in level
	vector <Sign> _signs; //stores signs in level
	vector <Item> _items; //stores level items
};

#endif