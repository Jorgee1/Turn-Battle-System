#include "Item.h"

Item::Item(){
    name = "";
    id = -1;
}

Item::Item(string item_name, int item_id){
    name = item_name;
    id = item_id;
}

void Item::action(){}

string Item::repr() const{
    return "<Item " + name +">";
}

NullItem::NullItem(){
    name = "None";
    id = 0;
}

Potion::Potion(){
    name = "Potion";
    id = 1;
}

void Potion::action(Character& character){
    character.set_hp(character.get_hp() + HEAL_AMOUNT);
}