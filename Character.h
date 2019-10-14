#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

#ifndef CHARACTER_H
#define CHARATCER_H

class Character{
    public:

        Character();
        Character(string, int, int, int, int);

        // Getters
        int get_hp() const;
        int get_max_hp() const;
        int get_attk() const;
        int get_def() const;
        int get_exp() const;
        string get_name() const;
        int get_next_level_exp() const;

        // Setters
        void set_hp(int);
               
        // Utility
        string repr() const;
        void add_exp(int);
        void display_character_stats() const;

    protected:
        string NAME;
        int HP, MAX_HP, ATTK, DEF; 
        int EXPEREIENCE;
        int NEXT_LEVEL;
        int LEVEL;

    private:
        void level_up();
};


#endif