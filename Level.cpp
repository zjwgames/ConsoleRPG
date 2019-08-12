
#include "Level.h"

#include <fstream>
#include <iostream>
#include <Windows.h>
#include <time.h>
#include <algorithm> //for_each
using namespace std;

// PUBLIC

Level::Level() {
	_currentLevel = 0;
	_playerDied = false;
}

void Level::load(string fileName, Player &player) {
	// Load level
	ifstream levelFile;
	levelFile.open(fileName);

	if (levelFile.fail()) { //If file failed to open...
		perror(fileName.c_str());
		printf("ERROR! File loading failed.");
		printf("\n[ENTER] to continue\n");
		cin.ignore();
		exit(1); //...exit game
	}

	string line;
	while (getline(levelFile, line)) { // Add file line by line to vector of level data
		_levelData.push_back(line);
	}

	levelFile.close();

	//Save starting configuration
	_levelStartData = _levelData;

	// Process level
	char tile;
	for (unsigned int i = 0; i < _levelData.size(); i++) { //Loop through y
		for (unsigned int j = 0; j < _levelData.size(); j++) { //Loop through x
			//Get current level tile
			tile = _levelData[i][j];
			switch (tile) { //Check tile for player/enemy/etc.
			case '@': // Player
				player.setPosition(j, i);
				break;
			case 'S': // Snake enemy
				_enemies.push_back(Enemy("Snake", tile, 1, 3, 1, 10, 50));
				_enemies.back().setPosition(j, i);
				break;
			case 'G': // Goblin enemy
				_enemies.push_back(Enemy("Goblin", tile, 2, 10, 5, 35, 150));
				_enemies.back().setPosition(j, i);
				break;
			case 'B': // Bandit enemy
				_enemies.push_back(Enemy("Bandit", tile, 3, 15, 10, 100, 250));
				_enemies.back().setPosition(j, i);
				break;
			case 'O': // Ogre enemy
				_enemies.push_back(Enemy("Ogre", tile, 4, 20, 40, 200, 500));
				_enemies.back().setPosition(j, i);
				break;
			case 'D': // Dragon enemy
				_enemies.push_back(Enemy("Dragon", tile, 100, 2000, 2000, 2000, 500000000));
				_enemies.back().setPosition(j, i);
				break;
			case '/': // Unlocked door
				_doors.push_back(Door("Unlocked Door", false));
				_doors.back().setPosition(j, i);
				break;
			case '%': // Locked door
				_doors.push_back(Door("Locked Door", true));
				_doors.back().setPosition(j, i);
				break;
			case '+': // Sign
				_signs.push_back(Sign("Sign", "Welcome to the dungeon!"));
				// Signs => string in construction should be from level list
				// Sign("Sign " + to_string(_currentLevel), _signMessages[currentLevel] )
				_signs.back().setPosition(j, i);
				break;
			case 'h': // Health potion item
				_items.push_back(Item("Health Potion", tile, true, Item::health, 20));
				_items.back().setPosition(j, i);
				break;
			case 'a': // Attack potion item
				_items.push_back(Item("Attack Potion", tile, true, Item::attack, 10));
				_items.back().setPosition(j, i);
				break;
			case 'd': //Defense potion item
				_items.push_back(Item("Defense Potion", tile, true, Item::defense, 10));
				_items.back().setPosition(j, i);
				break;
			case 'v': // Book item
				_items.push_back(Item("Book", tile, true, Item::experience, 100));
				_items.back().setPosition(j, i);
				break;
			case 'k': // Key item
				_items.push_back(Item("Key", tile, false, Item::none, 0));
				_items.back().setPosition(j, i);
				break;
			}
		}
	}
}

void Level::saveAsStartRoom() {
	string fileName = "level1.txt";
	ofstream newLevel;
	newLevel.open(fileName);
	for (unsigned int i = 0; i < _levelSize; i++) {
		newLevel << _levelStartData[i]; //Insert line by line
		newLevel << "\n";
	}
	newLevel.close();
	addMessage("Level saved as starting room!");
}

void Level::print(Player &player) {
	// Clear screen- using Windows.h
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleTextAttribute(console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
	SetConsoleCursorPosition(console, topLeft);

	// Print level data
	printLevel(_levelData);

	//Print player stats and depth (current level)
	// Get player stats
	vector <string> playerStats;
	string lineStats;
	lineStats = "Depth: " + to_string(_currentLevel);
	playerStats.push_back(lineStats);
	lineStats = "Level: " + to_string(player.getLevel());
	playerStats.push_back(lineStats);
	lineStats = "Health: " + to_string(player.getHealth());
	playerStats.push_back(lineStats);
	lineStats = "Attack: " + to_string(player.getAttack());
	playerStats.push_back(lineStats);
	lineStats = "Defense: " + to_string(player.getDefense());
	playerStats.push_back(lineStats);
	lineStats = "Experience: " + to_string(player.getExperience());
	playerStats.push_back(lineStats);

	// Print stats
	int xCoordStats = 46;
	int yCoordStats = 2;
	printUIData<vector<string>>(console, xCoordStats, yCoordStats, playerStats);

	//Print player inventory
	int xCoordInventory =  68;
	int yCoordInventory = 2;
	COORD inventoryCoord = { xCoordInventory, yCoordInventory };
	SetConsoleCursorPosition(console, inventoryCoord);
	printf("Inventory:");
	// Prints inventory contents
	yCoordInventory++;
	vector <string> playerInventory = player.getInventoryList();
	printUIInventory(console, xCoordInventory, yCoordInventory, playerInventory);

	//Print level messages
	// Check list is smaller than max no. of messages
	const int maxMessages = 18;
	if (_levelMessages.size() >= maxMessages) {
		for (unsigned int i = _levelMessages.size(); i > maxMessages; i--) {
			_levelMessages.pop_front(); // Remove oldest messages from list
		}
	}
	// Print out remaining list of messages
	int xCoordMessages = 46;
	int yCoordMessages = 12;
	COORD messagesCoord = { xCoordMessages, yCoordMessages };
	SetConsoleCursorPosition(console, messagesCoord);
	printUIData<list<string>>(console, xCoordMessages, yCoordMessages, _levelMessages);

	// Reset text colour
	SetConsoleTextAttribute(console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
	// Reset cursor below level
	COORD bottomLeft = { 0, _levelData.size() + 1 };
	SetConsoleCursorPosition(console, bottomLeft);
}

void Level::movePlayer(char input, Player &player) {

	int playerX, playerY;
	
	player.getPosition(playerX, playerY);

	switch (input) {
	case 'w': //up
	case 'W':
		processPlayerMove(player, playerX, playerY - 1);
		break;
	case 'a': //left
	case 'A':
		processPlayerMove(player, playerX - 1, playerY);
		break;
	case 's': //down
	case 'S':
		processPlayerMove(player, playerX, playerY + 1);
		break;
	case 'd': //right
	case 'D':
		processPlayerMove(player, playerX + 1, playerY);
		break;
	default:
		printf("INVALID INPUT!\n");
		printf("\n[ENTER] to continue\n");
		cin.ignore();
		break;
	}
}

void Level::useItemOnPlayer(int inventorySlot, Player &player) {
	//Check item is usable
	if (player.getItem(inventorySlot - 1).isUsable()) {
		//Use potion
		Item item;
		item += player.getItem(inventorySlot - 1);
		player.useItem(inventorySlot - 1);
		string itemName = item.getName();
		addMessage("Used " + itemName + "!");
	}
	else {
		printf("\nItem is not usable!\n");
	}
}

void Level::updateEnemies(Player &player) {
	char aiMove;
	int playerX, playerY;
	int enemyX, enemyY;
	player.getPosition(playerX, playerY);

	for (unsigned int i = 0; i < _enemies.size(); i++) {
		aiMove = _enemies[i].getMove(playerX, playerY);
		_enemies[i].getPosition(enemyX, enemyY);

		switch (aiMove) {
		case 'w': //up
			processEnemyMove(player, i, enemyX, enemyY - 1);
			break;
		case 'a': //left
			processEnemyMove(player, i, enemyX - 1, enemyY);
			break;
		case 's': //down
			processEnemyMove(player, i, enemyX, enemyY + 1);
			break;
		case 'd': //right
			processEnemyMove(player, i, enemyX + 1, enemyY);
			break;
		}
	}
}

char Level::getTile(int x, int y) {
	return _levelData[y][x];
}

unsigned int Level::getCurrentLevel() {
	return _currentLevel;
}

const unsigned int Level::getLevelSize() {
	return _levelSize;
}

bool Level::getPlayerDied() {
	return _playerDied;
}

void Level::setPlayerDied(bool isDead) {
	_playerDied = isDead;
}

void Level::setTile(int x, int y, char tile) {
	_levelData[y][x] = tile;
}

void Level::addMessage(const string message) {
	_levelMessages.push_back(message);
}


// PRIVATE

void Level::loadNextLevel(Player &player, bool hiddenLevel) {
	//Erase old level data
	_levelData.clear();
	_levelMessages.clear();
	_enemies.clear();
	_doors.clear();
	_signs.clear();
	_items.clear();

	//Open next level file
	if (hiddenLevel) {
		string fileName = "levelHidden.txt";

		// Load level
		ifstream file;
		file.open(fileName);

		if (file.fail()) { //If file failed to open...
			printf("ERROR! Hidden level not found...");
			cin.ignore();
		}
		else {
			load(fileName, player); //load hidden level
		}

		file.close();
	}
	else {
		_currentLevel++;
		string fileName = "level" + to_string(_currentLevel) + ".txt";

		// Load level
		ifstream file;
		file.open(fileName);

		if (file.fail()) { //If file failed to open...
			//...generate new level
			printf("No saved level found! Generating new level... ");
			generate(player);
			printf("\n[ENTER] to continue\n");
			cin.ignore();
		}
		else {
			load(fileName, player);
		}

		file.close();
	}
}

void Level::printLevel(vector <string> v)
{
	// lambda expression to print level vector 
	for_each(v.begin(), v.end(), [](string line)
	{
		printf("%s\n", line.c_str());
	});
}

template <class T>
void Level::printUIData(HANDLE console, unsigned int startX, unsigned int startY, T data) {
	//Set starting coord for printing to screen
	COORD topLeft = { startX, startY }; //top left of UI list to be printed
	SetConsoleCursorPosition(console, topLeft);

	for (auto const &line : data) {
		//Print line
		printf("%s", line.c_str());
		//Update console cursor position
		startY++;
		COORD topLeft = { startX, startY };
		SetConsoleCursorPosition(console, topLeft);
	}
}

void Level::printUIInventory(HANDLE console, unsigned int startX, unsigned int startY, vector<string> data) {
	//Set starting coord for printing to screen
	COORD topLeft = { startX, startY }; //top left of UI list to be printed
	SetConsoleCursorPosition(console, topLeft);

	unsigned int inventorySlotNumber = 1;
	for (auto const &line : data) {
		printf("[%i]-%s", inventorySlotNumber, line.c_str());
		startY++;
		COORD inventoryCoord = { startX, startY };
		SetConsoleCursorPosition(console, inventoryCoord);
		inventorySlotNumber++;
	}
}

void Level::generate(Player &player) {
	const int levelSize = _levelSize;
	char level[levelSize][levelSize];

	int TIME = (int)time(NULL); // Cast time(NULL) from time_t to int for srand
	srand(TIME);

	//Fill blank level
	for (unsigned int i = 0; i < levelSize; i++) {
		for (unsigned int j = 0; j < levelSize; j++) {
			level[i][j] = ' ';
		}
	}

	//Move controller around
	// placing floor tiles
	int cx = levelSize / 2;
	int cy = levelSize / 2;
	int cxStart = cx;
	int cyStart = cy;
	int controllerSteps = levelSize*levelSize;
	while (controllerSteps > 0) {
		level[cx][cy] = '.';

		int dir = (rand() % 4) + 1; // choose random direction (1-4)
		//cout << dir;
		switch (dir) {
		case 1: //Move down
			if (cy + 1 > 1 && cy + 1 < levelSize - 1) {
				cy++;
			}
			break;
		case 2: //Move up
			if (cy - 1 > 1 && cy - 1 < levelSize - 1) {
				cy--;
			}
			break;
		case 3: //Move left
			if (cx - 1 > 1 && cx - 1 < levelSize - 1) {
				cx--;
			}
			break;
		case 4: //Move right
			if (cx + 1 > 1 && cx + 1 < levelSize - 1) {
				cx++;
			}
			break;
		}
		--controllerSteps;
	}

	//Place wall tiles
	for (unsigned int i = 1; i < levelSize - 1; i++) {
		for (unsigned int j = 1; j < levelSize - 1; j++) {
			if (level[i][j] == '.') { //If on floor tile
				// If empty...
				if (level[i + 1][j] != '.') { //...to right
					level[i + 1][j] = '#';
				}
				if (level[i - 1][j] != '.') { //...to left
					level[i - 1][j] = '#';
				}
				if (level[i][j - 1] != '.') { //...above
					level[i][j - 1] = '#';
				}
				if (level[i][j + 1] != '.') { //...below
					level[i][j + 1] = '#';
				}
			}
		}
	}

	//Place items
	for (unsigned int i = 1; i < levelSize - 1; i++) {
		for (unsigned int j = 1; j < levelSize - 1; j++) {
			if (level[i][j] == '.') { //If on floor tile

				int placeItemChance = (rand() % 50) + 1; // 1-50 chance of placing item

				// If empty and within 1:100 chance...
				if (level[i + 1][j] == '.' && placeItemChance == 1) { //...to right
					Item randomItem = chooseRandomItem();
					level[i + 1][j] = randomItem.getTile();
				}
				//Reset chance of placing enemy
				placeItemChance = (rand() % 50) + 1;
				if (level[i - 1][j] == '.' && placeItemChance == 1) { //...to left
					Item randomItem = chooseRandomItem();
					level[i - 1][j] = randomItem.getTile();
				}
				placeItemChance = (rand() % 50) + 1;
				if (level[i][j - 1] == '.' && placeItemChance == 1) { //...above
					Item randomItem = chooseRandomItem();
					level[i][j - 1] = randomItem.getTile();
				}
				placeItemChance = (rand() % 50) + 1;
				if (level[i][j + 1] == '.' && placeItemChance == 1) { //...below
					Item randomItem = chooseRandomItem();
					level[i][j + 1] = randomItem.getTile();
				}
			}
		}
	}

	//Place enemies
	for (unsigned int i = 1; i < levelSize - 1; i++) {
		for (unsigned int j = 1; j < levelSize - 1; j++) {
			if (level[i][j] == '.') { //If on floor tile

				int placeEnemyChance = (rand() % 100) + 1; // 1-100 chance of placing enemy

				// If empty and within 1:100 chance...
				if (level[i + 1][j] == '.' && placeEnemyChance == 1) { //...to right
					Enemy randomEnemy = chooseRandomEnemy();
					level[i + 1][j] = randomEnemy.getTile();
				}
				//Reset chance of placing enemy
				placeEnemyChance = (rand() % 100) + 1;
				if (level[i - 1][j] == '.' && placeEnemyChance == 1) { //...to left
					Enemy randomEnemy = chooseRandomEnemy();
					level[i - 1][j] = randomEnemy.getTile();
				}
				placeEnemyChance = (rand() % 100) + 1;
				if (level[i][j - 1] == '.' && placeEnemyChance == 1) { //...above
					Enemy randomEnemy = chooseRandomEnemy();
					level[i][j - 1] = randomEnemy.getTile();
				}
				placeEnemyChance = (rand() % 100) + 1;
				if (level[i][j + 1] == '.' && placeEnemyChance == 1) { //...below
					Enemy randomEnemy = chooseRandomEnemy();
					level[i][j + 1] = randomEnemy.getTile();
				}
			}
		}
	}

	// Place sign randomly - once
	bool signPlaced = false;
	int placeSignChance = (rand() % 100);
	for (unsigned int i = 0; i < levelSize; i++) {
		for (unsigned int j = 0; j < levelSize; j++) {
			if (signPlaced == false && placeSignChance < 20 && level[i][j] == '.') {
				level[i][j] = '+';
				signPlaced = true;
			}
		}
	} // otherwise, do not place a sign in level

	//Place door in centre - at controller start
	level[cxStart][cyStart] = '/';

	// Place player at floor space '.' by final position of controller
	bool playerPlaced = false;
	//if controller's nearest neighbours are within level bounds...
	if ((cx - 1 > 0 && cx + 1 < levelSize) && (cy - 1 > 0 && cy + 1 < levelSize)) {
		//...loop through nearest neighbours to place player
		for (int i = cx - 1; i < cx + 1; i++) {
			for (int j = cy - 1; j < cy + 1; j++) {
				// If empty...
				if (level[i + 1][j] == '.' && playerPlaced == false) { //...to right
					level[i + 1][j] = '@';
					playerPlaced = true;
				}
				//Reset chance of placing enemy
				if (level[i - 1][j] == '.' && playerPlaced == false) { //...to left
					level[i - 1][j] = '@';
					playerPlaced = true;
				}
				if (level[i][j - 1] == '.' && playerPlaced == false) { //...above
					level[i][j - 1] = '@';
					playerPlaced = true;
				}
				if (level[i][j + 1] == '.' && playerPlaced == false) { //...below
					level[i][j + 1] = '@';
					playerPlaced = true;
				}
			}
		}
		if (playerPlaced == false) level[cx][cy] = '@';
	}
	else {
		level[cx][cy] = '@';
	}

	//Save generated level
	string fileName = "level" + to_string(_currentLevel) + ".txt";
	ofstream newLevel;
	newLevel.open(fileName);
	for (unsigned int i = 0; i < levelSize; i++) {
		for (unsigned int j = 0; j < levelSize; j++) {
			newLevel << level[i][j];//level[i][j] = ' ';
		}
		newLevel << "\n";
	}
	newLevel.close();
	//Load generated level
	load(fileName, player);
}

Enemy Level::chooseRandomEnemy() {
	int maxEnemies = MAX_ENEMIES;
	int chance = (rand() % maxEnemies); // 0-4 enemy types
	int weightedChance = (rand() % 100);
	Enemy randomEnemy; // += assigns copy to randomEnemy
	switch (chance) {
	case snake:
		if (weightedChance > 50) randomEnemy += Enemy("Snake", 'S', 1, 3, 1, 10, 50);
		break;
	case goblin:
		if (weightedChance > 60) randomEnemy += Enemy("Goblin", 'G', 2, 10, 5, 35, 150);
		break;
	case bandit:
		if (weightedChance > 70) randomEnemy += Enemy("Bandit", 'B', 3, 15, 10, 100, 250);
		break;
	case ogre:
		if (weightedChance > 80) randomEnemy += Enemy("Ogre", 'O', 4, 20, 40, 200, 500);
		break;
	case dragon:
		if (weightedChance > 99) randomEnemy += Enemy("Dragon", 'D', 100, 2000, 2000, 2000, 500000000);
		break;
	}
	return randomEnemy;
}

Item Level::chooseRandomItem() {
	int maxItems = MAX_ITEMS;
	int chance = (rand() % maxItems); // 0-3 items types
	int weightedChance = (rand() % 100);
	Item randomItem; // += assigns copy to randomEnemy
	switch (chance) {
	case healthPotion:
		if (weightedChance > 50) randomItem += Item("Health Potion", 'h', true, Item::health, 20);
		break;
	case attackPotion:
		if (weightedChance > 60) randomItem += Item("Attack Potion", 'a', true, Item::attack, 10);
		break;
	case defensePotion:
		if (weightedChance > 70) randomItem += Item("Defense Potion", 'a', true, Item::defense, 10);
		break;
	case book:
		if (weightedChance > 85) randomItem += Item("Book", 'v', true, Item::experience, 100);
		break;
	case key:
		if (weightedChance > 95) randomItem += Item("Key", 'k', false, Item::none, 0);
		break;
	}
	return randomItem;
}

void Level::processPlayerMove(Player &player, int targetX, int targetY) {
	int playerX, playerY;
	player.getPosition(playerX, playerY);

	char moveTile;
	moveTile = getTile(targetX, targetY);

	string input;
	switch (moveTile) { // --- Collision-checking ---
	case '#':
		//printf("You ran into a wall!\n");
		addMessage("You ran into a wall!");
		break;
	case '.':
		player.setPosition(targetX, targetY);
		setTile(playerX, playerY, '.');
		setTile(targetX, targetY, '@');
		break;
	case '/':
		addMessage("You ran into an unlocked door!");
		printf("\nDo you want to go to the next level [y/n]? ");
		// Get input
		getline(cin, input);
		if (input == "y" || input == "Y") {
			//change to next level
			addMessage("You continue to the next level.");
			loadNextLevel(player, false);
		}
		break;
	case '%':
		addMessage("You ran into a locked door!");
		if (player.hasKey()) {
			printf("\nDo you want to unlock this door [y/n]? ");
			// Get input
			getline(cin, input);
			if (input == "y" || input == "Y") {
				//change to hidden level
				addMessage("You continue to a hidden level.");
				// ADD - dragon level
				//loadNextLevel(player, true);
			}
		}
		break;
	case '+':
		addMessage("You ran into a sign!");
		addMessage(_signs.back().getName() + " reads: '" + _signs.back().getMessage() + "'");
		break;
	// Item collisions
	case 'h':
		if (player.getInventorySize() < player.getInventoryMax()) {
			pickupItem(player, targetX, targetY);
			player.setPosition(targetX, targetY);
			setTile(playerX, playerY, '.');
			setTile(targetX, targetY, '@');
			addMessage("You picked up a health potion!");
		}
		else {
			addMessage("Your inventory is full! Cannot pick up health potion.");
		}
		break;
	case 'a':
		if (player.getInventorySize() < player.getInventoryMax()) {
			pickupItem(player, targetX, targetY);
			player.setPosition(targetX, targetY);
			setTile(playerX, playerY, '.');
			setTile(targetX, targetY, '@');
			addMessage("You picked up an attack potion!");
		}
		else {
			addMessage("Your inventory is full! Cannot pick up attack potion.");
		}
		break;
	case 'd':
		if (player.getInventorySize() < player.getInventoryMax()) {
			pickupItem(player, targetX, targetY);
			player.setPosition(targetX, targetY);
			setTile(playerX, playerY, '.');
			setTile(targetX, targetY, '@');
			addMessage("You picked up a defense potion!");
		}
		else {
			addMessage("Your inventory is full! Cannot pick up defense potion.");
		}
		break;
	case 'k':
		if (player.getInventorySize() < player.getInventoryMax()) {
			pickupItem(player, targetX, targetY);
			player.setPosition(targetX, targetY);
			setTile(playerX, playerY, '.');
			setTile(targetX, targetY, '@');
			addMessage("You picked up a key!");
		}
		else {
			addMessage("Your inventory is full! Cannot pick up key.");
		}
		break;
	case 'v':
		if (player.getInventorySize() < player.getInventoryMax()) {
			pickupItem(player, targetX, targetY);
			player.setPosition(targetX, targetY);
			setTile(playerX, playerY, '.');
			setTile(targetX, targetY, '@');
			addMessage("You picked up a book!");
		}
		else {
			addMessage("Your inventory is full! Cannot pick up book.");
		}
		break;
	default: // Other
		battleEnemy(player, targetX, targetY);
		break;
	}
}

void Level::processEnemyMove(Player &player, int enemyIndex, int targetX, int targetY) {
	int playerX, playerY;
	int enemyX, enemyY;
	player.getPosition(playerX, playerY);
	_enemies[enemyIndex].getPosition(enemyX, enemyY);

	char moveTile;
	moveTile = getTile(targetX, targetY);

	switch (moveTile) {
	case '#': // Wall
		break;
	case '.':
		_enemies[enemyIndex].setPosition(targetX, targetY);
		setTile(enemyX, enemyY, '.');
		setTile(targetX, targetY, _enemies[enemyIndex].getTile());
		break;
	case '@':
		battleEnemy(player, enemyX, enemyY);
		break;
	default: // Other
		break;
	}
}

void Level::battleEnemy(Player &player, int targetX, int targetY) {
	int enemyX, enemyY;
	int playerX, playerY;
	int attackChance;
	int attackResult;
	string enemyName;

	player.getPosition(playerX, playerY);

	for (unsigned int i = 0; i < _enemies.size(); i++) {

		_enemies[i].getPosition(enemyX, enemyY);
		if (targetX == enemyX && targetY == enemyY) {
			enemyName = _enemies[i].getName();

			//Battle enemy

			// Player turn
			attackChance = player.attack();
			string playerTurnStr = "Attacked " + enemyName + " with roll of " + to_string(attackChance);
			//printf("\nAttacked %s with roll of %d\n", enemyName.c_str(), attackChance);
			addMessage(playerTurnStr);

			attackResult = _enemies[i].takeDamage(attackChance);

			if (attackResult != 0) { // Enemy died
				setTile(targetX, targetY, '.');
				print(player);
				addMessage(enemyName + " died!");
				//printf("%s died!\n", enemyName.c_str());

				//Remove the enemy
				_enemies[i] = _enemies.back();
				_enemies.pop_back();
				i--;

				bool levelUp = player.addExperience(attackResult);
				if (levelUp == true) {
					addMessage("LEVEL UP! Current level: " + to_string(player.getLevel()) );
				}

				return;
			}
			
			// Enemy turn
			attackChance = _enemies[i].attack();
			string enemyTurnStr = enemyName + " attacked player with a roll of " + to_string(attackChance);
			//printf("%s attacked player with a roll of %d\n", enemyName.c_str(), attackChance);
			addMessage(enemyTurnStr);

			attackResult = (player.takeDamage(attackChance));

			if (attackResult != 0) { // Player died
				setTile(playerX, playerY, 'x');
				print(player);
				printf("YOU DIED!\n");
				_playerDied = true;
				cin.ignore();
			}
		}
	}
}

void Level::pickupItem(Player &player, int targetX, int targetY) {
	int itemX, itemY;
	int playerX, playerY;
	string itemName;

	player.getPosition(playerX, playerY);

	for (unsigned int i = 0; i < _items.size(); i++) {

		_items[i].getPosition(itemX, itemY);
		itemName = _items[i].getName();

		if (targetX == itemX && targetY == itemY) {
			//Pickup item
			player.addItem(_items[i]);
			return;
		}
	}
	return;
}
