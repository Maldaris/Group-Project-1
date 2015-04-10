#pragma once
#include "stdafx.h"

#include "GameMap.h"
#include "Entities.h"

#include "Items.h"


using namespace std;
using namespace GameMap;
using namespace Entities;
#ifndef __ASSIST__
#define __ASSIST__
namespace Helpers {
	const xyPair StairCoords(INT_MIN, INT_MIN);
	bool randBool(){
		return rand() % 2 == 0;
	}
	Cardinal randomCardinal(){
		int r = rand() % 4;
		return r == 0 ? NORTH : r == 1 ? EAST : r == 2 ? SOUTH : WEST;
	}
	xyPair cardinalOffset(xyPair a, Cardinal card){
		switch (card){
		case NORTH:
			(*a[1] )++;
			break;
		case SOUTH:
			(*a[1])--;
			break;
		case EAST:
			(*a[0])++;
			break;
		case WEST:
			(*a[0])--;
		}
		return a;
	}
	Cardinal rotateRight(Cardinal card){
		if (card == NORTH){
			return EAST;
		}
		else if (card == EAST){
			return SOUTH;
		}
		else if (card == SOUTH){
			return WEST;
		}
		else if (card == WEST){
			return NORTH;
		}
		return NORTH;
	}
	Cardinal dirToAdjacent(Room* r1, Room* r2){
		if (!isAdjacent(r1, r2)) return NORTH;
		xyPair c1 = r1->getCoords();
		xyPair c2 = r2->getCoords();
		if (c1 == StairCoords || c2 == StairCoords)
			return STAIRS;
		if (*c1[0] > *c2[0])
			return WEST;
		else if (*c1[0] < *c2[0])
			return EAST;
		if (*c1[1] > *c2[1])
			return SOUTH;
		else
			return NORTH;
	}
	template<class T> bool vectorContains(vector<T*>* v, T* t){
		for (vector<T*>::iterator it = v->begin(); it != v->end(); ++it){
			if (*it == t)
				return true;
		} return false;
	}
	bool inRange(int a, int b, int range){
		if (abs(a - b) <= range){
			return true;
		}
		return false;
	}
	bool isAdjacent(Room* r1, Room* r2){
		if (r1 == NULL || r2 == NULL) return true;
		xyPair lcoords = r1->getCoords();
		xyPair rcoords = r2->getCoords();
		if (Helpers::inRange(*lcoords[0], *rcoords[0], 1))
			return true;
		if (Helpers::inRange(*lcoords[1], *rcoords[1], 1))
			return true;
		return false;
	}
	Cardinal getCardinalAdjacency(Room* a, Room* b){
		xyPair lcoords = a->getCoords();
		xyPair rcoords = b->getCoords();
		if (lcoords == StairCoords || rcoords == StairCoords)
			return STAIRS;
		if (*lcoords[0] < *rcoords[0])
			return Cardinal::EAST;
		if (*lcoords[0] > *rcoords[0])
			return Cardinal::WEST;
		if (*lcoords[1] > *rcoords[1])
			return Cardinal::SOUTH;
		if (*lcoords[1] < *rcoords[1])
			return Cardinal::NORTH;
		return Cardinal::NORTH;
	}
	Room* getRoomAtCardinal(Room* from, vector<Room*>* r, Cardinal card){
		for (vector<Room*>::iterator it = r->begin(); it != r->end(); ++it){
			if (Helpers::dirToAdjacent(from, (*it)) == card)
				return (*it);
		} 
		return nullptr;
	}
	GameMap::Stairwell* getStairwell(GameMap::Room* src){
		vector<Door*>* r = src->getDoors();
		for (vector<Door*>::iterator it = r->begin(); it != r->end(); ++it){
			Stairwell* isStairs = dynamic_cast<Stairwell*>((*it)->getOpposite(src));
			if (isStairs != NULL) return isStairs;
		} return nullptr;
	}
}
#endif
Room::Room(string _description, int x, int y): mDoors() , mEntities(), mx(x), my(y){
	mDescription = _description;
}
Room::Room(string _description, Helpers::xyPair rc) : mDoors(), mEntities(), mx(*rc[0]), my(*rc[1]){
	mDescription = _description;
}
Room::~Room(){
	if (!mDoors.empty()){
		for (unsigned int x = 0; x < mDoors.size(); x++){
			delete mDoors.at(x);
		}
	}
	if (!mEntities.empty()){
		for (unsigned int x = 0; x < mEntities.size(); x++){
			delete mEntities.at(x);
		}
	}
}
bool Room::addEntity(Entities::Entity* entity){
	mEntities.push_back(entity);
	return true;
}
bool Room::removeEntity(Entities::Entity* entity){
	bool found = false;
	for (vector<Entity*>::iterator it = mEntities.begin(); it != mEntities.end(); ++it){
		if (entity == *it){
			found = true;
			mEntities.erase(it);
			break;
		}
	}
	return found;
}
bool Room::addConnection(Door* door){
	Room* toConnect = door->getOpposite(this);
	if (Helpers::isAdjacent(toConnect, this)){
		Cardinal card = Helpers::getCardinalAdjacency(door->getOpposite(this), this);
		if (!this->doorAtCardinal(card)){
			mDoors.push_back(door);
			this->setDoorAtCardinal(card, true);
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}
bool Room::removeConnection(Door* door){
	bool found = false;
	if (mDoors.empty()) 
		return false;
	mDoors.erase(std::remove(mDoors.begin(), mDoors.end(), door), mDoors.end());
	return found;
}
bool Room::connected(Room* room){
	for (vector<Door*>::iterator it = mDoors.begin(); it != mDoors.end(); ++it){
		if ((*it)->getOpposite(this) == room)
			return true;
	} return false;
}
Helpers::xyPair Room::getCoords(){
	if (this == NULL) return Helpers::xyPair(-1, -1);
	Helpers::xyPair ret( this->mx, this->my );
	return ret;
}
void Room::describe(){
	std::cout << mDescription << endl;
}
bool Room::doorAtCardinal(Cardinal card){
	if (card == Cardinal::NORTH)
		return this->mn;
	if (card == Cardinal::SOUTH)
		return this->ms;
	if (card == Cardinal::EAST)
		return this->me;
	if (card == Cardinal::WEST)
		return this->mw;
	return NORTH;
}
void Room::setDoorAtCardinal(Cardinal card, bool state){
	if (card == Cardinal::NORTH)
		this->mn = state;
	if (card == Cardinal::SOUTH)
		this->ms = state;
	if (card == Cardinal::EAST)
		this->me = state;
	if (card == Cardinal::WEST)
		this->mw = state;
}
vector<Room*>* Room::getConnectedRooms(){
	vector<Room*>* ret = new vector<Room*>();
	for (vector<Door*>::iterator it = mDoors.begin(); it != mDoors.end(); ++it){
		ret->push_back((*it)->getOpposite(this));
	} return ret;
}
Door* Room::getConnectedDoors(Room* r){
	for (vector<Door*>::iterator it = mDoors.begin(); it != mDoors.end(); ++it){
		if ((*it)->getOpposite(this) == r)
			return *it;
	} return nullptr;
}
bool Room::containsEntity(Entity* e){
	return std::find(mEntities.begin(), mEntities.end(), e) != mEntities.end();
}
vector<Door*>* Room::getDoors(){
	return &mDoors;
}
void Room::dropItem(Items::Item* i){
	for (vector<Items::Container*>::iterator it = mItems.begin(); it != mItems.end(); ++it){
		if ((*it)->getName() == "Ground"){
			return (*it)->addItem(i);
		}
	}
}
void Room::addItemToContainer(string s, Items::Item* i){
	for (vector<Items::Container*>::iterator it = mItems.begin(); it != mItems.end(); ++it){
		if ((*it)->getName() == s){
			return (*it)->addItem(i);
		}
	}
}
vector<Items::Container*>* Room::getItemsOnGround(){
	return &mItems;
}
Items::Container* Room::pickupItem(Items::Item* i){
	for (vector<Items::Container*>::iterator it = mItems.begin(); it != mItems.end(); ++it){
		if (Helpers::vectorContains<Items::Item>((*it)->getItems(), i)){
			Items::Container* toDrop = new Items::Container("Box", CONTAINER_DESCRIPTION, i);
			return toDrop;
		}
	}
	return NULL;
}
EntryRoom::EntryRoom(string _description, int x , int y) : Room(_description, x , y){}
EntryRoom::EntryRoom(string _description, Helpers::xyPair rc) : Room(_description, rc){}
EntryRoom::~EntryRoom(){}
ExitRoom::ExitRoom(string _description, int x, int y) : Room(_description, x, y){}
ExitRoom::ExitRoom(string _description, Helpers::xyPair rc) : Room(_description, rc){}
ExitRoom::~ExitRoom(){}
Stairwell::Stairwell(string _description, EntryRoom* entry, ExitRoom* exit) : Room(_description, INT_MIN, INT_MIN),
upperRoom(entry), lowerRoom(exit){
	if (upperRoom != NULL){
		Door* uDoor = new Door(DOOR_DESCRIPTION, this, upperRoom);
		upperRoom->addConnection(uDoor);
		this->addConnection(uDoor);
	}
	if (lowerRoom != NULL){
		Door* lDoor = new Door(DOOR_DESCRIPTION, this, lowerRoom);
		this->addConnection(lDoor);
		lowerRoom->addConnection(new Door(DOOR_DESCRIPTION, this, lowerRoom));
	}
}
Stairwell::~Stairwell(){
	for (vector<Door*>::iterator it = mDoors.begin(); it != mDoors.end(); ++it){
		Door* door = *it;
		door->getOpposite(this)->removeConnection(door);
	}
	if (!mEntities.empty()){
		for (vector<Entity*>::iterator it = mEntities.begin(); it != mEntities.end(); ++it){
			delete *it;
		}
	}
}
int Stairwell::getUpperFloorNumber(){
	return upperFloor;
}
int Stairwell::getLowerFloorNumber(){
	return lowerFloor;
}
Door::Door(string _description, Room* a, Room* b):ma(a), mb(b), misLocked(false), misOpen(false), lockDifficulty(-1){
	mDescription = _description;
}
Door::Door(string _description, Room* a, Room* b, bool locked, bool open, int diff): ma(a),mb(b), misLocked(locked), misOpen(open), lockDifficulty(diff){
	mDescription = _description;
}
Door::~Door(){
	if (ma != NULL)
		ma->removeConnection(this);
	if (mb != NULL)
		mb->removeConnection(this);
}
bool Door::isOpen(){
	return misOpen;
}
bool Door::isUnlocked(){
	return !misLocked;
}
bool Door::attemptUnlock(int attempt){
	if (attempt >= lockDifficulty){
		misLocked = true;
		return true;
	} return false;
}
bool Door::open(){
	if (!misLocked){
		misOpen = true;
		return true;
	}
	return false;
}
Room* Door::getOpposite(Room* a){
	return (a == ma) ? mb : ma;
}
Room** Door::getConnections(){
	Room** ret = new Room*[2];
	ret[0] = ma;
	ret[1] = mb;
	return ret;
}
void Door::describe(){
	cout << mDescription << endl;
}
PortalDoor::PortalDoor(string _d, Room* a, Room* b) : Door(_d, a, b){}
PortalDoor::PortalDoor(string _d, Room* a, Room* b, bool l, bool o, int df) : Door(_d, a, b, l, o, df){}
Floor::Floor(string _desc){
	mDescription = _desc;
}
Floor::~Floor(){
	for (vector<Room*>::iterator it = rooms.begin(); it != rooms.end(); ++it){
		delete *(it);
	} 
}
bool Floor::addRoom(Room* r){
	if (!isOccupied(r)){
		rooms.push_back(r);
		return true;
	}
	else {
		return false;
	}
}
bool Floor::removeRoom(Room* r){
	bool found = false;
	for (vector<Room*>::iterator it = rooms.begin(); it != rooms.end(); ++it){
		if (r == *it){
			found = true;
			rooms.erase(it);
		}
	}
	return found;
}
EntryRoom* Floor::getEntryRoom(){
	return inRoom;
}
ExitRoom* Floor::getExitRoom(){
	return outRoom;
}
void Floor::setEntryRoom(Room* a){
	EntryRoom* t = dynamic_cast<EntryRoom*>(a);
	if (t != NULL)
		inRoom = t;
}
void Floor::setExitRoom(Room* b){
	this->outRoom = dynamic_cast<ExitRoom*>(b);
}
void Floor::describe(){
	cout << mDescription << endl;
}
Room* Floor::getVectorTail(){
	return rooms.back();
}
void Floor::shuffleVector(){
	std::random_shuffle(rooms.begin(), rooms.end());
}
vector<Room*>* Floor::getRooms(){
	return &rooms;
}
bool Floor::isOccupied(Room* r){
	Helpers::xyPair rc = r->getCoords();
	for (vector<Room*>::iterator it = rooms.begin(); it != rooms.end(); ++it){
		if ((*it)->getCoords() == rc)
			return true;
	} return false;
}
bool Floor::isOccupied(int x, int y){
	Helpers::xyPair rc(x,y);
	for (vector<Room*>::iterator it = rooms.begin(); it != rooms.end(); ++it){
		if ((*it)->getCoords() == rc)
			return true;
	} return false;
}
bool Floor::isOccupied(Helpers::xyPair rc){
	for (vector<Room*>::iterator it = rooms.begin(); it != rooms.end(); ++it){
		if ((*it)->getCoords() == rc)
			return true;
	} return false;
}
SpawnFloor::SpawnFloor(string _d) : Floor(_d){
	this->setExitRoom(new ExitRoom(EXIT_ROOM_DESCRIPTION, 0,0));
	this->setEntryRoom(new EntryRoom(ENTRY_ROOM_DESCRIPTION, 0, 1));
	Door* nd = new Door(DOOR_DESCRIPTION, this->getEntryRoom(), this->getExitRoom());
	this->getEntryRoom()->addConnection(nd);
	this->getExitRoom()->addConnection(nd);
}
Map::Map(){
	MATRIX_IMPLEMENTATION ? matrix_generate() : legacy_generate();
}
Map::~Map(){
	for (vector<Floor*>::iterator it = floors.begin(); it != floors.end(); ++it){
		delete *(it);
	}
}
#ifdef MATRIX_IMPLEMENTATION
#if MATRIX_IMPLEMENTATION == 1
void Map::matrix_generate(){
	arma::mat* toBuild = new arma::mat((FLOOR_COUNT) * FLOOR_COMPLEXITY, (FLOOR_COUNT) * FLOOR_COMPLEXITY, arma::fill::zeros);
}
#else
void Map::matrix_generate(){
	return;
}
#endif

#endif
void connectRooms(Room* r1, Room* r2){
	Door* d = new Door(DOOR_DESCRIPTION, r1, r2);
	r1->addConnection(d);
	r2->addConnection(d);
}
Floor* generateFloor(){
	Floor* ret = new Floor(FLOOR_DESCRIPTION);
	ret->setEntryRoom(new EntryRoom(ENTRY_ROOM_DESCRIPTION, 0, 0));
	ret->addRoom(ret->getEntryRoom());
	for (int x = FLOOR_COMPLEXITY - 2; x > 0; x--){
		bool added = false;
		do {
			Cardinal card = Helpers::randomCardinal();
			if (ret->getVectorTail()->doorAtCardinal(card) == false){
				Room* toAdd = new Room(ROOM_DESCRIPTION, Helpers::cardinalOffset(ret->getVectorTail()->getCoords(), card));
				connectRooms(ret->getVectorTail(), toAdd);
				ret->addRoom(toAdd);
				added = true;
			}
		} while (!added);
	}
	Helpers::xyPair tail = ret->getVectorTail()->getCoords();
	ret->setExitRoom(new ExitRoom(EXIT_ROOM_DESCRIPTION, (*tail[0]) + 1, *tail[1]));
	connectRooms(ret->getExitRoom(), ret->getVectorTail());
	ret->addRoom(ret->getExitRoom());

	return ret;
}
Stairwell* connectFloors(Floor* upper, Floor* lower){
	ExitRoom* u = upper->getExitRoom();
	EntryRoom* l = lower->getEntryRoom();
	return new Stairwell(STAIRWELL_DESCRIPTION, l, u);
}
void Map::legacy_generate(){
	spawn = new SpawnFloor(SPAWN_FLOOR_DESCRIPTION);
	Floor* toadd = generateFloor();
	stairs.push_back(connectFloors(spawn, toadd));
	floors.push_back(toadd);
	for (int x = FLOOR_COUNT - 1; x > 0; x--){
		toadd = generateFloor();
		floors.push_back(toadd);
		stairs.push_back(connectFloors(floors.back(), toadd));
	}
}
SpawnFloor* Map::getSpawnFloor(){
	return spawn;
}
vector<Floor*>* Map::getFloors(){
	return &floors;
}
vector<Stairwell*>* Map::getStairs(){
	return &stairs;
}
Floor* Map::findFloorWithRoom(Room* r){
	for (vector<Floor*>::iterator it = this->floors.begin(); it != this->floors.end(); ++it){
		if (std::find((*it)->getRooms()->begin(), (*it)->getRooms()->end(), r) != (*it)->getRooms()->end())
			return (*it);
	} return nullptr;
}