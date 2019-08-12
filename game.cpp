#include <iostream>
#include <string>

#include "GameEngine.h"

using namespace std;

/*
 --- ADVANCED FEATURES ---
 -STL containers - lists (_levelMessages - allows top one to be deleted pop_front)
 -enums
 -file I/O
 -Inheritance
 -Polymorphism
 -Headers and implementations
 -Lambda functions
 -Templates
 -Overloading operators (+=)

 --- TODO ---

 -Add key and locked door functionality
 -Make getter for _gameOver in GameEngine (game._gameOver => game.getGameOver())

 -Balance enemies

 -Add tile legend - what does each character in level represent?

 -Add namespaces, dynamic memory allocation (new/delete),
  smart pointers, etc.

 -Finish signs  -  Polish sign placement / messages => see Level::load

 -Make player and enemies inherit from level object

 -Use base class pointer for locked/unlocked door ?
 


 -Add inventory item that allows wall destruction (bomb)

 -Finish level generator => place items, improve sign placement

 Extensions:

 -Add enemy drop item
 -Balance enemies and player levelling => progression
 -Polish up enemy defn -> Level::print and Level::chooseRandomEnemy both contain
						  enemy definitions => encapsulate to one universal defn
 
 -Add signs that tell story throughout
 -Add keys to unlock locked doors
 -Improve level generator

 -Add more advanced functionality

 -Add colour? SetColorAttribute(console, color)

*/

int main() {

	bool gameOver = false;

	while (!gameOver) {
		GameEngine game("level0.txt"); //Initialise engine (with first level)

		game.start();
		
		gameOver = game._gameOver;

		if (gameOver) { // if game is over ask if player wants to restart
			string input;
			while (true) {
				printf("\nRestart game [y/n]? \n");
				cin >> input;
				if (input == "y" || input == "Y") {
					gameOver = false;
					break;
				}
				else if (input == "n" || input == "N") {
					gameOver = true;
					printf("\nGame exiting...\n");
					cin.ignore();
					break;
					//exit(1);
				}
				printf("\n [y] to restart\n [n] to exit\n");
			}
		}
	}

	return 0;
}