#ifndef LEVELOBJECT_H
#define LEVELOBJECT_H

#pragma once

#include <string>
using namespace std;

class LevelObject { //abstract class for any type of object placed in level
public:
	LevelObject();

	LevelObject(string name);

	//Setters
	virtual void setName(string name); //sets level object name
	virtual void  setPosition(int x, int y); //sets level object position

	//Getters
	virtual string getName(); //gets level object name
	virtual void const getPosition(int &x, int &y); //gets level object position

protected:
	//Properties
	string _name; //stores level object name
	int _x; //stores level object x-coord in level
	int _y; //stores level object y-coord in level
};

#endif