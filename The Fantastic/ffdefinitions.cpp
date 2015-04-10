#include "stdafx.h"
#include "ffdefinitions.h"

namespace Helpers {
	int randIntInRange(int min, int max){
		return min + (rand() % (int)(max - min + 1));
	}
	string cardToString(Cardinal card){
		if (card == NORTH)
			return "North";
		if (card == SOUTH)
			return "South";
		if (card == EAST)
			return "East";
		if (card == WEST)
			return "West";
		if (card == STAIRS)
			return "Stairs";
		return "North";
	}
	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}
	std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, elems);
		return elems;
	}
	string merge(vector<string> s){
		string ret = "";
		for (unsigned int x = 0; x < s.size(); x++)
			ret += s.at(x);
		return ret;
	}
	void print(vector<string> s){
		for (unsigned int x = 0; x < s.size(); x++)
			cout << s.at(x) << " ";
		cout << endl;
	}
}
namespace Globals {
	Entities::Player* GlobalPlayerPtr = nullptr;
	GameMap::Map* GlobalMapPtr = nullptr;
	vector<Entities::Monster*>* GlobalMonsterVector = nullptr;
};