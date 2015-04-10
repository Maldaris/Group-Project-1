#pragma once
#include <vector>
#include <string>
#include "ffdefinitions.h"
using namespace std;
namespace Items {
	class Item : public Describable	{
	public:
		inline Item(string _n, string _d): mName(_n), mCount(1){
			mDescription = _d;
		}
		inline Item(string _n, string _d, int c): mName(_n), mCount(c){
			mDescription = _d;
		}
		inline ~Item(){};
		inline void setUseCount(int i){ mCount = i; };
		inline int getUseCount(){ return mCount; };
		inline void use(){ 
			mCount--;
			cout << "You use " << mName << endl;
			cout << ITEM_USE_DESCRIPTION << endl;
		};
		inline string getName() {
			return mName;
		}
	private:
		string mName;
		int mCount;
	};
	inline bool operator== (Item lhs, Item rhs){
		return lhs.getName() == rhs.getName();
	};
	class Container : public Describable {
	public:
		Container(string, string, Item*);
		~Container();
		vector<Items::Item*>* getItems();
		bool contains(Item* i);
		string getName();
		void addItem(Items::Item* i);
		bool itemMerge(Container* c);
		void describe();
	private:
		vector<Items::Item*> mItems;
		string mName;
	};
}