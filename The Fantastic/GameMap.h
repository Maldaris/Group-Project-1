#pragma once
#define __GAMEMAP_H__
#include "stdafx.h"
#include <iostream>
#include <map>
#include <array>
#include <vector>
#include "ffdefinitions.h"

using namespace std;
using namespace Entities;
namespace Helpers{
	class xyPair {
	private:
		int x, y;
	public:
		inline xyPair(int a , int b){
			this->x = a;
			this->y = b;
		}
		inline int* operator[](int index){
			if (index == 0) return &x;
			if (index == 1) return &y;
			return NULL;
		}
		inline bool operator==(xyPair rhs){
			if (rhs.x == this->x && rhs.y == this->y)
				return true;
			return false;
		}
	};
	bool randBool();
	Cardinal randomCardinal();
	xyPair cardinalOffset(xyPair, Cardinal);
	bool isAdjacent(GameMap::Room* r1, GameMap::Room* r2);
	Cardinal dirToAdjacent(GameMap::Room*,GameMap::Room*);
	GameMap::Room* getRoomAtCardinal(GameMap::Room* from, vector<GameMap::Room*>* r, Cardinal card);
	GameMap::Stairwell* getStairwell(GameMap::Room* src);
}
namespace GameMap {
	class Room : Describable {
	public:
		Room(string description, int x, int y);
		Room(string description, Helpers::xyPair rc);
		~Room();
		vector<Entity*>* getEntities(){ return &mEntities; };
		bool addEntity(Entity*);
		bool removeEntity(Entity*);
		bool containsEntity(Entity*);
		bool addConnection(Door*);
		bool removeConnection(Door*);
		bool connected(Room*);
		vector<Room*>* getConnectedRooms();
		Door* getConnectedDoors(Room*);
		vector<Door*>* getDoors();
		Helpers::xyPair getCoords();
		void describe();
		vector<Items::Container*>* getItemsOnGround();
		void dropItem(Items::Item*);
		void addItemToContainer(string s, Items::Item* i);
		Items::Container* pickupItem(Items::Item*);
		bool doorAtCardinal(Cardinal);
		void setDoorAtCardinal(Cardinal, bool);
	protected:
		vector<Door*> mDoors;
		vector<Entity*> mEntities;
		vector<Items::Container*> mItems;
		int mx, my, eSize;
		bool mn, ms, me, mw;
	};
	class EntryRoom : public Room {
	public:
		EntryRoom(string _description, int x, int y);
		EntryRoom(string _d, Helpers::xyPair rc);
		~EntryRoom();
	};
	class ExitRoom : public Room {
	public:
		ExitRoom(string _description, int x, int y);
		ExitRoom(string _d, Helpers::xyPair rc);
		~ExitRoom();
	};
	class Stairwell : public Room {
	public:
		Stairwell(string _description, EntryRoom* , ExitRoom*);
		~Stairwell();
		int getUpperFloorNumber();
		int getLowerFloorNumber();
	protected:
		int upperFloor, lowerFloor;
		EntryRoom* upperRoom;
		ExitRoom* lowerRoom;
	};
	class Door : Describable {
	public:
		Door(string, Room*, Room*);
		Door(string, Room*, Room*, bool, bool, int);
		~Door();
		bool isOpen();
		bool isUnlocked();
		bool attemptUnlock(int);
		bool open();
		Room* getOpposite(Room*);
		Room** getConnections();
		void describe();
	protected:
		Room* ma, *mb;
		bool misLocked;
		bool misOpen;
		int lockDifficulty;
	};
	class PortalDoor : Door {
		PortalDoor(string, Room*, Room*);
		PortalDoor(string, Room*, Room*, bool, bool, int);
	};
	class Floor : Describable {
	public:
		Floor(string);
		~Floor();
		bool addRoom(Room*);
		bool removeRoom(Room*);
		virtual EntryRoom* getEntryRoom();
		void setEntryRoom(Room* a);
		virtual ExitRoom* getExitRoom();
		void setExitRoom(Room* b);
		void describe();
		void shuffleVector();
		Room* getVectorTail();
		vector<Room*>* getRooms();
		bool isOccupied(Room*);
		bool isOccupied(int x, int y);
		bool isOccupied(Helpers::xyPair);
	private:
		EntryRoom* inRoom;
		ExitRoom* outRoom;
		vector<Room*> rooms;
	};
	class SpawnFloor : public Floor {
	public:
		SpawnFloor(string);
	};
	class Map {
	public:
		Map();
		~Map();
		void legacy_generate();
		void matrix_generate();
		Floor* findFloorWithRoom(Room* r);
		SpawnFloor* getSpawnFloor();
		vector<Floor*>* getFloors();
		vector<Stairwell*>* getStairs();
	private:
		SpawnFloor* spawn;
		vector<Floor*> floors;
		vector<Stairwell*> stairs;
	};
}
