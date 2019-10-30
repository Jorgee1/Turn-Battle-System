#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>

using namespace std;

#ifndef CHARACTER_H
#define CHARACTER_H

class Status{
    public:
        Status(string, int);
        string get_name() const;
        int get_value() const;
        void set_value(int);
        void add_value(int);
    private:
        string name;
        int value;
};

class Character{
    public:
        int HP       = 0;
        int MAX_HP   = 1;
        int ATTACK   = 2;
        int DEFFENCE = 3;

        Character();
        void init(string, int, int, int, int);

        // Getters
        Status get_hp() const;
        Status get_max_hp() const;
        Status get_attk() const;
        Status get_def() const;
        string get_name() const;
        vector<Status> get_stats() const;

        // Setters
        void set_hp(int);
               
        // Utility
        string repr() const;

    protected:
        string NAME;
        vector<Status> character_stats;
        int LEVEL;
};

class Hero: public Character{
    public:
        int EXPEREIENCE = 4;
        int NEXT_LEVEL  = 5;

        Hero();
        void init(string, int, int, int, int);
        void add_exp(int);
        Status get_exp() const;
        Status get_next_level_exp() const;
        string repr() const;
        
    private:
        void level_up();
};

class Monster: public Character{
    public:
        Monster();
        void init(string, int, int, int, int, int);
        int get_exp_reward() const;
    private:
        int EXP_REWARD;
};

#endif