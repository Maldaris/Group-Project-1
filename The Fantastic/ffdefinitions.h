#pragma once

#include "stdafx.h"

using namespace std;
enum Cardinal {
	NORTH, SOUTH, EAST, WEST, STAIRS
};
namespace GameMap {
	class Room;
	class Door;
	class Floor;
	class Map;
	class Stairwell;

};
namespace Entities {
	class Entity;
	class Player;
	class Monster;
};
namespace Items {
	class Item;
	class Container;
}
class Describable {
public:
	virtual void describe() {
		if (mDescription == "")
			std::cout << "There is SOMETHING here, but it's beyond your understanding." << endl;
		else
			std::cout << mDescription << endl;
	};
	string mDescription;
};
namespace Helpers {
	int randIntInRange(int min, int max);
	string cardToString(Cardinal card);
	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
	std::vector<std::string> split(const std::string &s, char delim);
	std::string merge(vector<string> t);
	void print(vector<string> t);
}
namespace Globals {
	extern Entities::Player* GlobalPlayerPtr;
	extern GameMap::Map* GlobalMapPtr;
	extern vector<Entities::Monster*>* GlobalMonsterVector;
};
#ifndef __PREPROCESSOR_DEFINITIONS__
#define __PREPROCESSOR_DEFINITIONS__
#define FLOOR_COUNT 4
#define FLOOR_COMPLEXITY 10
#define MAX_TRAIL_LENGTH 4
#define DOOR_DESCRIPTION "A heavy wood and iron door, possibly locked. The scratching on the other end makes you think it should stay shut."
#define ROOM_DESCRIPTION "It doesn't matter how many of these dungeon rooms you see, they're no less creepier than the last..."
#define SPAWN_FLOOR_DESCRIPTION "A towering entrance to a creepy dungeon. You're going in anyways."
#define ENTRY_ROOM_DESCRIPTION "A reprieve from the stairs, the room is only slightly bigger. And might have monsters. Into the fire, eh?"
#define FLOOR_DESCRIPTION "A dark, musty dungeon floor. It's littered with moss, bones, and several unspeakables."
#define STAIRWELL_DESCRIPTION "A spiral stairwell. No handrails makes it almost as lethal as the dungeon itself."
#define EXIT_ROOM_DESCRIPTION "The room centerpieces stairs that spiral down further into the deep. Hope you're not claustrophobic."
#define CONTAINER_DESCRIPTION "A non descrit cardboard box."
#define PLAYER_DESCRIPTION "Do I really have to describe you? Here's a mirror, do it yourself."
#define MONSTER_DESCRIPTON "Beady red eyes. Snarling mouth. Big hands. And it wants to eat you."
#define MAX_HP 10
#define ITEM_USE_DESCRIPTION "The widget spins. Weeee."
#define MATRIX_IMPLEMENTATION 0
#endif
