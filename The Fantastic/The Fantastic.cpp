// The Fantastic.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <string>
using namespace std;

#include "stdafx.h"
#include "ffdefinitions.h"
#include "GameMap.h"
#include "Entities.h"
#include "Items.h"
using namespace GameMap;

void look(vector<string>);
void cmove(vector<string>);
void attack(vector<string>);
void tickOthers();
int _tmain(int argc, _TCHAR* argv[])
{
	srand(time(0));
	bool terminus = false;
	Globals::GlobalMapPtr = new Map();
	Globals::GlobalPlayerPtr = new Player(PLAYER_DESCRIPTION, Globals::GlobalMapPtr->getSpawnFloor()->getEntryRoom());
	Globals::GlobalMonsterVector = new vector<Monster*>();
	
	Globals::GlobalMonsterVector->push_back(new Monster("Bob", MONSTER_DESCRIPTON, Globals::GlobalMapPtr->getFloors()->at(
		Helpers::randIntInRange(0, Globals::GlobalMapPtr->getFloors()->size()-1))->getVectorTail()));
	Globals::GlobalMonsterVector->push_back(new Monster("Dave", MONSTER_DESCRIPTON, Globals::GlobalMapPtr->getFloors()->at(
		Helpers::randIntInRange(0, Globals::GlobalMapPtr->getFloors()->size() - 1))->getVectorTail()));
	Globals::GlobalMonsterVector->push_back(new Monster("Steve", MONSTER_DESCRIPTON, Globals::GlobalMapPtr->getFloors()->at(
		Helpers::randIntInRange(0, Globals::GlobalMapPtr->getFloors()->size() - 1))->getVectorTail()));
	while (!terminus){
		cout << "?> ";
		string input;
		getline(cin, input);
		//cout << input << endl;;
		vector<string> tokens = Helpers::split(input, ' ');
		//Helpers::print(tokens);
		if (tokens.size() > 0){
			if (tokens.at(0) == "l" || tokens.at(0) == "look"){
				tokens.erase(tokens.begin());
				look(tokens);
			}
			else if (tokens.at(0) == "m" || tokens.at(0) == "move"){
				tokens.erase(tokens.begin());
				cmove(tokens);
				tickOthers();
			}
			else if (tokens.at(0) == "a" || tokens.at(0) == "attack"){
				tokens.erase(tokens.begin());
				attack(tokens);
				tickOthers();
			}
			else if (tokens.at(0) == "q" || tokens.at(0) == "quit"){
				terminus = !terminus;
			}
			else if (tokens.at(0) == "h" || tokens.at(0) == "help"){
				cout << "Commands: \"move\", \"look\", \"attack\", \"help\", or \"quit\"" << endl;
				cout << "each command can be used either with the full word or the first letter." << endl;
				cout << "move can also take a parameter, if it's a valid direction." << endl;
				cout << "ex: m west" << endl;
			}
		}

		cin.sync();
	}
}
void tickOthers(){
	for (vector<Monster*>::iterator it = Globals::GlobalMonsterVector->begin(); Globals::GlobalMonsterVector->end() != it; ++it)
		if (*it != nullptr)
			(*it)->tick();
}
void  cmove(vector<string> to){
	if (to.size() == 0){
		cout << "Move to where?" << endl << "?> ";
		string input;
		cin >> input;
		if (input == "n" || input == "north"){
			to.push_back("n");
			cmove(to);
		}
		else if (input == "s" || input == "south"){
			to.push_back("s");
			cmove(to);
		}
		else if (input == "e" || input == "east"){
			to.push_back("e");
			cmove(to);
		}
		else if (input == "w" || input == "west"){
			to.push_back("w");
			cmove(to);
		}
		else {
			cout << "Not a valid direction!" << endl;
		}
	}
	else {
		Room* curRoom = Globals::GlobalPlayerPtr->getCurrentRoom();
		if (to.at(0) == "n" || to.at(0) == "north"){
			Room* dest = Helpers::getRoomAtCardinal(curRoom, curRoom->getConnectedRooms(), NORTH);
			if (dest == nullptr) return;
			Door* destDoor = curRoom->getConnectedDoors(dest);
			if (destDoor->isUnlocked()){
				Globals::GlobalPlayerPtr->moveToRoom(destDoor->getOpposite(curRoom));
				cout << "You move to the north." << endl;
			}
			else {
				cout << "The door's locked!" << endl;
			}
		}
		else if (to.at(0) == "s" || to.at(0) == "south"){
			Room* dest = Helpers::getRoomAtCardinal(curRoom, curRoom->getConnectedRooms(), SOUTH);
			if (dest == nullptr) return;
			Door* destDoor = curRoom->getConnectedDoors(dest);
			if (destDoor->isUnlocked()){
				Globals::GlobalPlayerPtr->moveToRoom(destDoor->getOpposite(curRoom));
				cout << "You move to the south." << endl;
			}
			else {
				cout << "The door's locked!" << endl;
			}
		}
		else if (to.at(0) == "e" || to.at(0) == "east"){
			Room* dest = Helpers::getRoomAtCardinal(curRoom, curRoom->getConnectedRooms(), EAST);
			if (dest == nullptr) return;
			Door* destDoor = curRoom->getConnectedDoors(dest);
			if (destDoor->isUnlocked()){
				Globals::GlobalPlayerPtr->moveToRoom(destDoor->getOpposite(curRoom));
				cout << "You move to the east." << endl;
			}
			else {
				cout << "The door's locked!" << endl;
			}
		}
		else if (to.at(0) == "w" || to.at(0) == "west"){
			Room* dest = Helpers::getRoomAtCardinal(curRoom, curRoom->getConnectedRooms(), WEST);
			if (dest == nullptr) return;
			Door* destDoor = curRoom->getConnectedDoors(dest);
			if (destDoor->isUnlocked()){
				Globals::GlobalPlayerPtr->moveToRoom(destDoor->getOpposite(curRoom));
				cout << "You move to the west." << endl;
			}
			else {
				cout << "The door's locked!" << endl;
			}
		}
		else if (to.at(0) == "stairs"){
			Room* dest = Helpers::getStairwell(curRoom);
			if (dest == nullptr) return;
			Room* prev = curRoom;
			Globals::GlobalPlayerPtr->moveToRoom(dest);
			curRoom = Globals::GlobalPlayerPtr->getCurrentRoom();
			if (curRoom->getConnectedRooms()->empty())
				return;
			for (unsigned int x = 0; x < curRoom->getConnectedRooms()->size(); x++){
				if (curRoom->getConnectedRooms()->at(x) != prev){
					dest = curRoom->getConnectedRooms()->at(x);
					break;
				}
			}
			Globals::GlobalPlayerPtr->moveToRoom(dest);
			cout << "You move down the stairs." << endl;
		}
		else {
			cout << "Not a valid direction! Try again." << endl;
		}
	}
}
void attack(vector<string> tgt){
	Room* curRoom = Globals::GlobalPlayerPtr->getCurrentRoom();
	if (curRoom->getEntities()->size() == 1){
		cout << "You're the only one here. And I don't think you want to hit yourself." << endl;
		return;
	}
	unsigned int index;
	bool success = false;
	cout << "There are " << curRoom->getEntities()->size() << "entities here. Which one?" << endl << "?> ";
	while (!success){
		cin >> index;
		if ((int)index >= 0 && index < curRoom->getEntities()->size()){
			Monster* m = dynamic_cast<Monster*>(curRoom->getEntities()->at(index));
			if (m == NULL){
				cout << "Doesn't look like that one's a monster, try another." << endl;
			}
			else {
				m->setHP(m->getHP() - 2);
				if (m->getHP() <= 0)
					cout << "You killed " << m->mName << "!" << endl;
				else
					cout << "You deal 2 damage to " << m->mName << "." << endl;
				success = true;
			}
		}
		else {
			cout << "There's no mob at that number. Try again." << endl;
		}
		index = -1;
	}
}
void look(vector<string> at){
	Room* curRoom = Globals::GlobalPlayerPtr->getCurrentRoom();
	if (at.size() == 0){
		cout << "Looking around you, you see..." << endl;
		curRoom->describe();
		if (!curRoom->getEntities()->empty())
			for (vector<Entity*>::iterator it = ++curRoom->getEntities()->begin(); it != curRoom->getEntities()->end(); ++it){
				Monster* m = dynamic_cast<Monster*>((*it));
				if (m != nullptr){
					m->describe();
				}
				else {
					(*it)->describe();
				}
			}
		else
			cout << "You're the only being in the room." << endl;
		if (!curRoom->getItemsOnGround()->empty())
			for (vector<Items::Container*>::iterator it = curRoom->getItemsOnGround()->begin(); it != curRoom->getItemsOnGround()->end(); ++it)
				(*it)->describe();
		else
			cout << "The room is devoid of containers with loot. Drats!" << endl;
		if (!curRoom->getDoors()->empty())
			for (vector<Door*>::iterator it = curRoom->getDoors()->begin(); it != curRoom->getDoors()->end(); ++it){
				cout << "There is a door to the " << Helpers::cardToString(Helpers::dirToAdjacent(curRoom, (*it)->getOpposite(curRoom))) << ". ";
				cout << (((*it)->isUnlocked()) ? "It's unlocked." : "It's locked.") << endl;
			}
	}
	else {
		if (at.at(0) == "door"){
			if (!curRoom->getDoors()->empty()){
				if (curRoom->getDoors()->size() == 1){
					Door* it = curRoom->getDoors()->at(0);
					cout << "There is a door to the " << Helpers::cardToString(Helpers::dirToAdjacent(curRoom, (it)->getOpposite(curRoom))) << ". ";
					cout << (((it)->isUnlocked()) ? "It's unlocked." : "It's locked.") << endl;
				}
				else {
					unsigned int index;
					bool success = false;
					cout << "There are " << curRoom->getDoors()->size() << " doors here. Which one?" << endl << "?> ";
					while (!success) {
						cin >> index;
						if (index >= 0 && index < curRoom->getDoors()->size()){
							Door* it = curRoom->getDoors()->at(index);
							cout << "There is a door to the " << Helpers::cardToString(Helpers::dirToAdjacent(curRoom, (it)->getOpposite(curRoom))) << ". ";
							cout << (((it)->isUnlocked()) ? "It's unlocked." : "It's locked.") << endl;
							success = true;
						}
						else {
							cout << "There's no door at that number. Try again." << endl;
						}
						index = -1;
					}
				}
			}
			else {
				cout << "That's odd. There aren't any doors here. You're trapped!" << endl;
			}
		}
		else if (at.at(0) == "box"){
			if (curRoom->getItemsOnGround()->size() > 0){
				if (curRoom->getItemsOnGround()->size() == 1){
					Items::Container* c = curRoom->getItemsOnGround()->at(0);
					c->describe();
				}
				else {
					unsigned int index;
					bool success = false;
					cout << "There are " << curRoom->getItemsOnGround()->size() << " containers here. Which one?" << endl << "?> ";
					while (!success){
						cin >> index;
						if ((int)index >= 0 && index < curRoom->getItemsOnGround()->size()){
							curRoom->getItemsOnGround()->at(index)->describe();
							success = true;
						}
						else {
							cout << "There's no container at that number. Try again." << endl;
						}
						index = -1;
					}
				}
			}
			else {
				cout << "The room is devoid of containers with loot. Drats!" << endl;
			}
		}
		else if (at.at(0) == "mob"){
			if (curRoom->getEntities()->size() > 1){ //you're in the room, and apparently so are mobs.
				if (curRoom->getEntities()->size() == 2){
					//cout << "dbg: in solo entity" << endl;
					Monster* m = dynamic_cast<Monster*>(curRoom->getEntities()->at(1));
					if (m != nullptr){
						m->describe();
					} else {
						curRoom->getEntities()->at(1)->describe();
					}
				}
				else {
					unsigned int index;
					bool success = false;
					cout << "There are " << curRoom->getEntities()->size() - 1 << " mobs here. Which one?" << endl << "?> ";
					while (!success){
						cin >> index;
						if ((int)index >= 0 && index < curRoom->getEntities()->size()){
							Monster* m = dynamic_cast<Monster*>(curRoom->getEntities()->at(index + 1));
							if (m != nullptr)
								m->describe();
							else
								curRoom->getEntities()->at(index + 1)->describe();
							success = true;
						}
						else {
							cout << "There's no mob at that number. Try again." << endl;
						}
						index = -1;
					}
				}
			}
			else {
				cout << "You're alone in the room. You hear a tiny violin play." << endl;
			}
		}
		else {
			at.erase(at.begin());
			return look(at);
		}
	}
}