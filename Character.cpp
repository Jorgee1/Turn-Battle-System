#include "Character.h"

Character::Character(){
    NAME = "None";
    MAX_HP = 1;
    HP = MAX_HP;
    ATTK = 0;
    DEF = 0;
    EXPEREIENCE = 0;
    NEXT_LEVEL = 10;
    LEVEL = 1;
}

Character::Character(string name, int max_hp, int attack, int defense, int level){
    NAME = name;
    MAX_HP = max_hp;
    HP = MAX_HP;
    ATTK = attack;
    DEF = defense;
    EXPEREIENCE = 0;
    NEXT_LEVEL = 10;
    LEVEL = level;
}

// Getters
int Character::get_hp() const { return HP; }
int Character::get_max_hp() const { return MAX_HP; }
int Character::get_attk() const { return ATTK; }
int Character::get_def() const { return DEF; }
int Character::get_exp() const { return EXPEREIENCE; };
string Character::get_name() const { return NAME; };
int Character::get_next_level_exp() const { return NEXT_LEVEL; };

// Setters
void Character::set_hp(int value){
    if (value < 0){
        HP = 0;
    }else if (value > MAX_HP){
        HP = MAX_HP;
    }else{
        HP = value;
    }
}

// Util

string Character::repr() const{
    return "<Character " + NAME + " : " + "[" + to_string(HP) + "/" + 
    to_string(MAX_HP) + "]  LVL " + to_string(LEVEL) + ">";
}

void Character::add_exp(int value){
    while(value>0){
        if (NEXT_LEVEL > value + EXPEREIENCE){
            EXPEREIENCE += value;
            value = 0;
        }else{
            value = value - NEXT_LEVEL + EXPEREIENCE;
            EXPEREIENCE += NEXT_LEVEL;
            
            level_up();
        }
    }
    
}

void Character::display_character_stats() const{
    cout << endl;
    cout << get_name() << " - LEVEL: " << LEVEL << endl;
    cout << "HP      : " << get_hp() << " / " << get_max_hp() << endl;
    cout << "ATACK   : " << get_attk() << endl;
    cout << "DEFENCE : " << get_def() << endl;
    cout << "EXP     : " << get_exp() << "/" << get_next_level_exp() << endl;
    cout << endl;
}

void Character::level_up(){
    LEVEL += 1;
    MAX_HP += 5;
    ATTK += 1;
    DEF += 1;
    EXPEREIENCE = 0;
    NEXT_LEVEL += 2;
}