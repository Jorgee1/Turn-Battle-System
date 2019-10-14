#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>


using namespace std;

#ifndef ITEM_H
#define ITEM_H

#include "Character.h"

class Item{
    public:
        Item();
        Item(string, int);
        void action();
        string repr() const;

    protected:
        string name;
        int id;
};

class NullItem: public Item{
    public:
        NullItem();
};

class Potion: public Item{
    public:
        Potion();
        void action(Character&);

    private:
        int HEAL_AMOUNT = 5;
};

#endif