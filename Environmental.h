#ifndef ENVIRONMENTAL_H
#define ENVIRONMENTAL_H

#pragma once
#include "LevelObject.h"

class Environmental : public LevelObject { //Abstract class for general environmental level object
public:
	Environmental(string name);
	// Virtual Destructor
	virtual ~Environmental(); // ensures base class destruction also destructs derived classes too

	//Setters
	void setName(string name); //sets environmental level object name
	void setPosition(int x, int y); //sets environmental level object position

	// Getters
	string getName();  //gets environmental level object name
	void const getPosition(int &x, int &y);  //gets environmental level object position
};

// --- DOOR ---

class Door : public Environmental {
public:
	// Parameterised Constructor
	Door(string name, bool locked);

	// Destructor
	~Door();

private:
	double _locked; //stores if door is locked or not
};

// --- SIGN ---

class Sign : public Environmental {
public:
	// Parameterised Constructor
	Sign(string name, string message);

	// Destructor
	~Sign();

	// Getters
	string getMessage(); //gets sign message

private:
	string _message; //stores sign message
};

#endif