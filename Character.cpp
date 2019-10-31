#include "Character.h"

// Status

Status::Status(string name, int value){
    this->name = name;
    this->value = value;
}

string Status::get_name() const{ return name; }
int Status::get_value() const{ return value; }
void Status::set_value(int value) { this->value = value; }
void Status::add_value(int value) { this->value+= value; }
// Characters

Character::Character(){
    NAME = "";
    character_stats.push_back(Status("HP", 1));
    character_stats.push_back(Status("Max_HP", 1));
    character_stats.push_back(Status("Attack", 1));
    character_stats.push_back(Status("Defence", 1));
    LEVEL = 1;
}

void Character::init(string name, int max_hp, int attack, int defense, int level){
    NAME = name;
    character_stats[HP].set_value(max_hp);
    character_stats[MAX_HP].set_value(max_hp);
    character_stats[ATTACK].set_value(attack);
    character_stats[DEFFENCE].set_value(defense);
    LEVEL = level;
}

// Getters
Status Character::get_hp()     const { return character_stats[HP];       }
Status Character::get_max_hp() const { return character_stats[MAX_HP];   }
Status Character::get_attk()   const { return character_stats[ATTACK];   }
Status Character::get_def()    const { return character_stats[DEFFENCE]; }
string Character::get_name()   const { return NAME; };
vector<Status> Character::get_stats() const{ return character_stats; }

// Setters
void Character::set_hp(int value){
    if (value < 0){
        character_stats[HP].set_value(0);
    }else if (value > character_stats[MAX_HP].get_value()){
        character_stats[HP].set_value(character_stats[MAX_HP].get_value());
    }else{
        character_stats[HP].set_value(value);
    }
}

// Util

string Character::repr() const{
    return "<Character " + NAME + ": " + "[" + to_string(character_stats[HP].get_value()) + "/" + 
    to_string(character_stats[MAX_HP].get_value()) + "]  LVL " + to_string(LEVEL) + ">";
}

// Hero
Hero::Hero(){
    character_stats.push_back(Status("Experience", 0));
    character_stats.push_back(Status("Next Level", 10));
}

void Hero::init(string name, int max_hp, int attack, int defense, int level){
    NAME = name;
    character_stats[HP].set_value(max_hp);
    character_stats[MAX_HP].set_value(max_hp);
    character_stats[ATTACK].set_value(attack);
    character_stats[DEFFENCE].set_value(defense);
    character_stats[EXPEREIENCE].set_value(0);
    character_stats[NEXT_LEVEL].set_value(10);
    LEVEL = level;
}

Status Hero::get_exp() const { return character_stats[EXPEREIENCE]; };
Status Hero::get_next_level_exp() const { return character_stats[NEXT_LEVEL]; };

void Hero::add_exp(int value){
    while(value>0){
        if(value >= character_stats[NEXT_LEVEL].get_value()-character_stats[EXPEREIENCE].get_value()){
            value = value - (character_stats[NEXT_LEVEL].get_value()-character_stats[EXPEREIENCE].get_value());
            level_up();
        }else{
            character_stats[EXPEREIENCE].add_value(value);
            value = 0;
        }
    }
    
}

void Hero::level_up(){
    LEVEL += 1;
    character_stats[MAX_HP].add_value(5);
    character_stats[ATTACK].add_value(1);
    character_stats[DEFFENCE].add_value(1);
    character_stats[EXPEREIENCE].set_value(0);
    character_stats[NEXT_LEVEL].add_value(2);
}

string Hero::repr() const{
    return "<Hero " + NAME + " : " + "[" + to_string(character_stats[HP].get_value()) + "/" + 
    to_string(character_stats[MAX_HP].get_value()) + "]  LVL " + to_string(LEVEL) + ">";
}

// Monster
Monster::Monster(){
    EXP_REWARD = 0;
}

void Monster::init(string name, int max_hp, int attack, int defense, int level, int reward_exp){
    NAME = name;
    character_stats[HP].set_value(max_hp);
    character_stats[MAX_HP].set_value(max_hp);
    character_stats[ATTACK].set_value(attack);
    character_stats[DEFFENCE].set_value(defense);
    character_stats.push_back(Status("Defence", defense));
    LEVEL = level;
    EXP_REWARD = reward_exp;
}

int Monster::get_exp_reward() const{
    return EXP_REWARD;
}