#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

#ifndef INVENTORY_H
#define INVENTORY_H


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
        //void action(Character&);

    private:
        int HEAL_AMOUNT = 5;
};

class Inventory{
    public:
        Inventory();
        string repr() const;

    private:
        static const int total_items = 5;
        Item items[total_items];
};

#endif