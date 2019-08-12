#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#pragma once

#include <string>

using namespace std;

#include "Player.h"
#include "Level.h"

class GameEngine {
public:
	// Parameterised constructor - initialises GameEngine with first level
	GameEngine(string levelFileName);

	void start(); //starts game - contains main game loop
	void playerInput(); //takes in player input, feeds into level
public:
	bool _gameOver; //stores whether or not game has finished
private:
	Player _player; //stores game player
	Level _level; //stores game level
	int _state; //stores current player state - move/inventory
	enum playerState { //enumerates player states
		move,
		inventory
	};
};

#endif