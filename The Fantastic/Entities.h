#pragma once
#include "stdafx.h"
#include "Items.h"
#ifndef __GAMEMAP_H__
#include "GameMap.h"
#endif
using namespace GameMap;

namespace Helpers {
	bool playerInRoom(Room* r);
	Room* getRoomWithEntity(vector<Room*>* rooms, Entity* e);
#if DIST_IMPL_COMPLETE == 1
	int dist(Floor* floor, Room* src, Room* dest);
	queue<Room*>* routeTo(Room* src, Room* dest);
	void routeToRecurse(queue<Room*>* q, Room* step, Room* dest);
#endif
};
namespace Entities {
	class Entity : public Describable {
	public:
		Entity(string, string, Room*);
		Entity(string, string,  Room*, Items::Container*);
		~Entity();
		void moveToRoom(Room*);
		Room* getCurrentRoom();
		int getHP();
		void setHP(int a);
		Items::Container* getInventory();
		bool addItem(Items::Container* c);
		bool removeItem(Items::Item* i, int count);
		virtual void die();
		string mName;
	protected:
		Items::Container* mInventory;
		int mHP;
		Room* mCurrentRoom;
	};
	class Player : public Entity {
	public:
		Player(string, Room*);
		Player(string, Room*, Items::Container*);
		~Player();
		void die();
	};
	class Monster : public Entity {
	public:
		Monster(string _n, string _d, Room* _c);
		void tick();
		bool attack(Entity* e);
		void flee();
		void describe();
	private:
		int mDamage;
		int mToHit;
		Room* mLastRoom;
	};
}