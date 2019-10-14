#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>


using namespace std;

#ifndef INVENTORY_H
#define INVENTORY_H

#include "Item.h"

class Inventory{
    public:
        Inventory();
        string repr() const;

    private:
        static const int total_items = 5;
        Item items[total_items];
};

#endif