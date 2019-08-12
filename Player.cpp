#include "Player.h"
//#include <vector>
#include <string>
#include <random>
#include <ctime>
using namespace std;

//Constructor
Player::Player() : LevelObject() {
}

//Init player properties
void Player::init(int level, int health, int attack, int defense, int experience) {
	_level = level;
	_health = health;
	_attack = attack;
	_defense = defense;
	_experience = experience;
}

//Sets position of player
void Player::setPosition(int x, int y) {
	_x = x;
	_y = y;
}

//Get position of player using reference variables
void const Player::getPosition(int &x, int &y) {
	x = _x;
	y = _y;
}

//Gets stats
int const Player::getLevel() {
	return _level;
}

int const Player::getHealth() {
	return _health;
}

int const Player::getAttack() {
	return _attack;
}

int const Player::getDefense()  {
	return _defense;
}

int const Player::getExperience() {
	return _experience;
}

vector <string> Player::getInventoryList() {
	vector <string> inventoryList;
	for (unsigned int i = 0; i < _inventory.size(); i++) {
		inventoryList.push_back(_inventory[i].getName());
	}
	return inventoryList;
}

unsigned int Player::getInventorySize() {
	return _inventory.size();
}

unsigned int Player::getInventoryMax() {
	return _inventoryMax;
}

Item Player::getItem(int index) {
	return _inventory[index];
}

bool Player::hasKey() {
	for (unsigned inventorySlot = 0; inventorySlot < _inventory.size(); inventorySlot++) {
		if (_inventory[inventorySlot].getName() == "Key") {
			// Search succeeded
			return true;
		}
	}
	// Search failed
	return false;
}

int Player::attack() {
	int TIME = (int) time(NULL);
	static default_random_engine randomEngine(TIME);
	uniform_int_distribution<int> attackChance(0, _attack); // STL template

	return attackChance(randomEngine);
}

int Player::takeDamage(int attack) {
	attack -= _defense;
	//Check if attack did damage
	// (must be greater than player defense value)
	if (attack > 0) {
		_health -= attack;
		if (_health <= 0) {
			return 1; // player died
		}
		else {
			return 0; // player still alive
		}
	}
	else {
		return 0; // player still alive
	}
}

bool Player::addExperience(int experience) { //if levelled up true -> return true
	_experience += experience;
	bool levelUp = false;
	// Level up!
	while (_experience > 50) {
		_experience -= 50;
		_attack += 10;
		_defense += 5;
		_health += 10;
		_level++;
		levelUp = true;
	}
	return levelUp;
}

void Player::addItem(Item &item) {
	_inventory.push_back(item); // push back item based on type e.g. potion, etc.
}

void Player::useItem(int inventorySlot) {
	Item item = getItem(inventorySlot);
	//Item effect player
	if (item.isUsable()) {
		printf(" Slot %i - %s used!", inventorySlot + 1, item.getName().c_str());
		//Use item
		switch (item.getEffect()) {
		case Item::health:
			_health += item.getMagnitude();
			printf("\nHealth increased by: %i\n", item.getMagnitude());
			break;
		case Item::attack:
			_attack += item.getMagnitude();
			printf("\nAttack increased by: %i\n", item.getMagnitude());
			break;
		case Item::defense:
			_defense += item.getMagnitude();
			printf("\nDefense increased by: %i\n", item.getMagnitude());
			break;
		case Item::experience:
			bool levelUp = addExperience(item.getMagnitude());
			printf("\nExperience increased by: %i\n", item.getMagnitude());
			if (levelUp == true) {
				printf("LEVEL UP! Current level: %i", _level);
			}
			break;
		}
		_inventory.erase(_inventory.begin() + inventorySlot);
	}
	else {
		printf("\nItem is not usable!\n");
	}
}