
#pragma once
#include "stdafx.h"
#include "ffdefinitions.h"
#include "Entities.h"

using namespace GameMap;
using namespace Items;
using namespace Entities;
bool Helpers::playerInRoom(Room* r){
	for(vector<Entity*>::iterator it = r->getEntities()->begin();it != r->getEntities()->end(); ++it){
		if(dynamic_cast<Player*>((*it)) != nullptr)
			return true;
	}
	return false;
}
Room* Helpers::getRoomWithEntity(vector<Room*>* rooms, Entity* e){
	for (vector<Room*>::iterator it = rooms->begin(); it != rooms->end(); ++it){
		if ((*it)->containsEntity(e))
			return (*it);
	} return nullptr; 
}
#if DIST_IMPL_COMPLETE == 1
int Helpers::dist(Floor* floor, Room* src, Room* dest){
	vector<Room*>* rooms = floor->getRooms();
	if (std::find(rooms->begin(), rooms->end(), dest) == rooms->end())
		return INT_MAX;
	

}

queue<Room*>* Helpers::routeTo(Room* src, Room* dest){
	queue<Room*>* ret = new queue<Room*>();
	routeToRecurse(ret, src, dest);
	return ret;
}
void Helpers::routeToRecurse(queue<Room*>* q, Room* step, Room* dest){
	vector<Room*> srcLinks = *(step->getConnectedRooms());
	if (std::find(srcLinks.begin(), srcLinks.end(), dest) != srcLinks.end()){
		q->push(dest);
		return;
	}
	Room* shortest = nullptr;
	int ldist = INT_MAX;
	for (vector<Room*>::iterator it = srcLinks.begin(); it != srcLinks.end(); ++it){
		if (dist(Globals::GlobalMapPtr->findFloorWithRoom(*it), (*it), dest) < ldist){
			shortest = (*it);
			ldist = dist(Globals::GlobalMapPtr->findFloorWithRoom(*it), (*it), dest);
		}
	}
	q->push(shortest);
	return routeToRecurse(q, shortest, dest);
}
#endif
Entity::Entity(string _n, string _d, Room* r) {
	mCurrentRoom = r;
	mDescription = _d;
	mName = _n;
	this->mHP = MAX_HP;
	mCurrentRoom->addEntity(this);
}
Entity::Entity(string _n, string _d, Room* r, Items::Container* c) : mCurrentRoom(r), mInventory(c){
	mDescription = _d;
	mName = _n;
	this->mHP = MAX_HP;
}
Entity::~Entity(){}
void Entity::moveToRoom(Room* r){
	if (this->mCurrentRoom->connected(r)) { 
		this->mCurrentRoom->removeEntity(this);
		r->addEntity(this);
	}
	this->mCurrentRoom = r;
}
int Entity::getHP(){
	return mHP;
}
Room* Entity::getCurrentRoom(){
	return mCurrentRoom;
}
Items::Container* Entity::getInventory(){
	return mInventory;
}
bool Entity::addItem(Items::Container* c){
	return mInventory->itemMerge(c);
}
bool Entity::removeItem(Items::Item* i, int count){
	if (this->mInventory->contains(i)){
		vector<Item*>::iterator tgt = std::find(mInventory->getItems()->begin(), mInventory->getItems()->end(), i);
		if ((*tgt)->getUseCount() < count)
			return false;
		if ((*tgt)->getUseCount() == count){
			mInventory->getItems()->erase(tgt);
		}
		else {
			(*tgt)->setUseCount((*tgt)->getUseCount() - count);
		} return true;
	} return false;
}
void Entity::setHP(int i){
	this->mHP = i;
}
void Entity::die(){
	cout << "The creature croaks!" << endl;
	delete this;
}
Player::Player(string _d, Room* _c): Entity("Player", _d, _c){}
Player::Player(string _d, Room* _c, Items::Container* _i) : Entity("You",_d, _c, _i){}
Player::~Player(){
	cout << "You lose!" << endl;
	cin.sync();
	cin.get();
	exit(0);
}
void Player::die(){
	cout << "You die!" << endl;
	delete this;
}
Monster::Monster(string _n, string _d, Room* _c) : Entity(_n, _d, _c) {
	this->mDamage = 20;
	this->mToHit = 12;
}
void Monster::tick(){
	if (this->getHP() < 2){
		this->flee();
	}
	if (this->mCurrentRoom == nullptr){
		for (unsigned int x = 0; x < Globals::GlobalMapPtr->getFloors()->size(); x++){
			Room* r = Helpers::getRoomWithEntity(Globals::GlobalMapPtr->getFloors()->at(x)->getRooms(), this);
			if (r != nullptr){
				this->mCurrentRoom = r;
			}
		}
	}
	if(Helpers::playerInRoom(this->mCurrentRoom)){
		this->attack(Globals::GlobalPlayerPtr);
	} else {
		//since I cba to actually figure out dist in time, randomly move, and camp the player.
		vector<Room*>* possibles = this->getCurrentRoom()->getConnectedRooms();
		if (possibles->size() > 1){
			this->moveToRoom(possibles->at(Helpers::randIntInRange(0, possibles->size() - 1)));
		}
		else if(possibles->size() == 1){
			this->moveToRoom(0);
		}
		this->setHP(this->getHP() + 1);
	}
}
/*
	tries to flee without boxing itself in.
*/
void Monster::flee(){
	
	vector<Room*> possibles = (*this->getCurrentRoom()->getConnectedRooms());
	while (1){
		if (possibles.size() == 1)
			return this->moveToRoom(possibles.at(0));
		Room* randpick = possibles.at(Helpers::randIntInRange(0, possibles.size()));
		if (randpick->getConnectedRooms()->size() != 1) 
			return this->moveToRoom(randpick);
		possibles.erase(std::find(possibles.begin(), possibles.end(), randpick));
	}
}
bool Monster::attack(Entity* e){
	if (Helpers::randIntInRange(0, 20) >= this->mToHit){
		cout << this->mName << " hit " << e->mName << " for " << this->mDamage << " damage!" << endl;
		if (e->getHP() <= this->mDamage){
			e->die();
		}
		else {
			e->setHP(this->getHP() - this->mDamage);
		}
		return true;
	} return false;
}
void Monster::describe(){
	cout << "There's a monster here, named " << mName << endl;
	cout << mDescription << endl << endl;
}