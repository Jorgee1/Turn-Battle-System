#include <string>

#include "common.h"
#include "texture.h"
#include "character.h"

using namespace std;

#ifndef LAYOUT_H
#define LAYOUT_H

class BattleSystem{
    public:
        Hero *player;
        Monster *monster;
        
        BattleSystem();
        ~BattleSystem();

        void set_player(Hero &);
        void set_enemy(Monster &);

        string player_attack();
        string enemy_attack();
        string player_heal();

        void restore_characters();
        bool check_battle_end();

    private:
        int victory_count;
        int defeat_count;
        int RECUPERATE_AMOUNT = 5;

        int damage_formula(int , int);
}; 

class BattleLayout{
    public:
        vector<string> mesages;
        BattleSystem battle;
        TextureText *text;

        const enum ACTIONS{
            ATACK,
            HEAL,
            STATUS,

            TOTAL_ACTIONS
        };

        string action_names[TOTAL_ACTIONS] = {
            "ATTACK",
            "HEAL",
            "STATUS"
        };

        int selector;

        const Uint8* currentKeyStates;
        int* layout_current;
        controls* controls_rules;
        controls_locks* locks;

        BattleLayout();
        ~BattleLayout();

        BattleLayout(int &, controls &, controls_locks &, TextureText []);

        void init(int &, controls &, controls_locks &, TextureText []);
        void next_option();
        void previews_option();
        void action();
        void check_player_action();

        void render(int x, int y);

};

class StatsLayout{
    public:
        TextureText *text;
        const Uint8* currentKeyStates;
        int* layout_current;
        controls* controls_rules;
        controls_locks* locks;
        Hero* player;

        StatsLayout();
        ~StatsLayout();

        StatsLayout(int &, controls &, controls_locks &, TextureText []);
        void init(int &, controls &, controls_locks &, TextureText []);

        void set_player(Hero &);
        void action();

        void check_player_action();
        void render(int, int);
};

class GameOverLayout{
    public:
        TextureText *text;
        const Uint8* currentKeyStates;
        int* layout_current;

        controls* controls_rules;
        controls_locks* locks;

        const enum ACTIONS{
            YES,
            NO,

            TOTAL_ACTIONS
        };

        string action_names[TOTAL_ACTIONS] = {
            "YES",
            "NO"
        };

        int selector;
        bool exit;

        GameOverLayout();
        ~GameOverLayout();

        GameOverLayout(int &, controls &, controls_locks &, TextureText []);
        void init(int &, controls &, controls_locks &, TextureText []);


        void next_option();
        void previews_option();
        bool check_exit();
        void action();

        void check_player_action();
        void render(int, int);

};

#endif