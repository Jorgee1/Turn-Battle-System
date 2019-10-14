#include "Inventory.h"


// Inventory
Inventory::Inventory(){
    for(int i=0; i<total_items; i++){
        NullItem none;
        items[i] = none;
    }
    Potion potion;
    items[0] = potion;
}

string Inventory::repr() const{
    string temp = "<Inventory:\n";
    for(int i=0; i<total_items; i++){
        temp+= "    " + to_string(i) + " = " + items[i].repr() + "\n";
    }
    return temp + ">\n";
}
