#pragma once
#include "stdafx.h"
#include "Items.h"
using namespace Items;
Container::Container(string _n, string _d, Item* i){
	this->mName = _n;
	this->mDescription = _d;
	this->mItems.push_back(i);
	
}
Container::~Container(){
}
vector<Item*>* Container::getItems(){
	return &mItems;
}
bool Container::contains(Item* i){
	/*
	if (std::find(mItems.begin(), mItems.end(), *i) != mItems.end())
		return true;
	*/
	return false;
}
string Container::getName(){ return mName; }
void Container::addItem(Item* i){
	mItems.push_back(i);
}
bool Container::itemMerge(Container* c){
	vector<Item*> toadd = *(c->getItems());
	for(vector<Item*>::iterator it = toadd.begin(); it != toadd.end(); ++it){
		vector<Item*>::iterator inInv = std::find(mItems.begin(), mItems.end(), *it);
		if(inInv != mItems.end()) {
			(*inInv)->setUseCount((*inInv)->getUseCount()+(*it)->getUseCount());
		} else {
			mItems.push_back((*it));
		}
	}
	return true;
}
void Container::describe(){
	cout << mDescription << endl;
	cout << "Inside you see..." << endl;
	for (unsigned int x = 0; x < mItems.size(); x++){
			cout << x << ". ";
			mItems.at(x)->describe();
			cout << endl;
	}
}