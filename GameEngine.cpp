#include "GameEngine.h"
#include "Level.h"
#include <iostream>
#include <sstream>
#include <conio.h>
#include <Windows.h>
#include <fstream>
using namespace std;

GameEngine::GameEngine(string levelFilename) {
	// Set up game
	_gameOver = false;
	_player.init(1, 100, 10, 10, 0);
	_state = move; // Current player state - move controls

	ifstream firstLevelFile(levelFilename);
	if (firstLevelFile.good()) { //if file exists, load file
		firstLevelFile.close();
		_level.load(levelFilename, _player);
	}
	if (firstLevelFile.fail()) { // if file does not exist - create start room
		ofstream firstLevel;
		firstLevel.open(levelFilename);
		// Fill level with blank spaces
		static const unsigned int levelSize = 32;
		char firstLevelData[levelSize][levelSize];
		for (unsigned int x = 0; x < levelSize; x++) {
			for (unsigned int y = 0; y < levelSize; y++) {
				firstLevelData[y][x] = '.';
			}
		}
		//Place player
		firstLevelData[levelSize / 2][levelSize / 2] = '@';
		//Place door
		firstLevelData[levelSize / 2][(levelSize / 2) + 4] = '/';

		for (unsigned int x = 0; x < levelSize; x++) {
			for (unsigned int y = 0; y < levelSize; y++) {
				firstLevel << firstLevelData[x][y]; //Fill file
			}
			firstLevel << endl;
		}

		firstLevel.close();
		_level.load(levelFilename, _player);
	}

	printf("\nGame loaded!\n");
	printf("\n[ENTER] to continue\n");
	cin.ignore();
}

void GameEngine::start() {

	// Init game window
	HWND console = GetConsoleWindow();
	RECT ConsoleRect;
	GetWindowRect(console, &ConsoleRect);
	MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 800, 700, TRUE);

	// Game loop
	while (!_gameOver) {
		// Main game
		_level.print(_player);
		playerInput();
		_level.updateEnemies(_player);

		if (_level.getPlayerDied()) {
			printf("\nGAME OVER!\n");
			_gameOver = true;
		}
	}

	//Clean up after game
	for (unsigned int level = 2; level <= _level.getCurrentLevel(); level++) {
		string filename = ("level" + to_string(level) + ".txt");
		string newFilename = ("oldlevel" + to_string(level) + ".txt");
		fstream file(newFilename.c_str());
		if (file.good()) { //if file exists...
			file.close();
			remove(newFilename.c_str()); //...remove oldLevel file
		}
		rename(filename.c_str(), newFilename.c_str()); //Rename level played to oldLevel
		remove(filename.c_str()); //Remove level played
	}
}

void GameEngine::playerInput() {
	char input;
	string quitInput;
	if (_state == move) {
		printf("Move/Attack [w/a/s/d] - Use item [i] - Save room [l] - Quit [q] ");
		input = _getch(); //gets input character, does not require ENTER to be pressed like cin
		switch (input) {
		case 'q':
		case 'Q':
			while (true) {
				printf("\nAre you sure you want to quit [y/n]?\n");
				cin >> quitInput;
				if (quitInput == "y" || quitInput == "Y") {
					printf("Quitting...");
					_level.setPlayerDied(true);
					break;
				}
				else if (quitInput == "n" || quitInput == "N") {
					printf("Continuing...");
					_level.setPlayerDied(false);
					break;
				}
				printf("\n[y] to quit\n[n] to continue\n");
			}
			break;
		case 'l':
		case 'L':
			_level.saveAsStartRoom();
			break;
		case 'i':
		case'I':
			// Allow inventory access
			_state = inventory;
			break;
		default:
			_state = move;
			_level.movePlayer(input, _player);
		}
	}

	string inventoryInput;
	if (_state == inventory) {
		_level.print(_player);
		printf("Type inventory slot number to use that item ([x] to cancel): ");
		while (true) {
			cin >> inventoryInput;

			//Test if integer
			stringstream ss(inventoryInput);
			unsigned int inventorySlot;
			if (ss >> inventorySlot) {
				//Check item slot exists in inventory
				if (inventorySlot <= _player.getInventorySize() && inventorySlot > 0) {
					// Use item on player
					_level.useItemOnPlayer(inventorySlot, _player);
					printf("\n[ENTER] to continue\n");
					cin.ignore();
					cin.ignore();
					_state = move;
					break;
				}
				else {
					//Inventory slot is empty or does not exist
					//Print error message
					printf(" ERROR! Inventory slot does not exist ");
					printf("\n[ENTER] to continue\n");
					cin.ignore();
					cin.ignore();
					break;
				}
			}
			else {
				if (inventoryInput == "x" || inventoryInput == "X") { //Stop using inventory
					_state = move;
					break;
				}
				else {
					//Print error message
				}
			}
		}
	}
}
