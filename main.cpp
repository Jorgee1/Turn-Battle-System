#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

#include "Character.h"
#include "Inventory.h"


/* TODO:
    Inventario
    Estado
*/

using namespace std;


class Menu{
    public:
        const int ATACK = 1;
        const int HEAL = 2;
        const int STATUS = 3;
        const int INVENTORY = 4;

        const int YES = 1;
        const int NO = 2;

        void display_battle_options() const{
            display_options(battle_options, n_battle_options);
        }

        void display_bool_options() const{
            display_options(bool_options, n_bool_options);
        }

        int get_option(){
            int opcion;
            cin >> opcion;
            return opcion;
        }

    private:
        const static int n_battle_options = 4;
        const static int n_bool_options = 2;
        const string battle_options[n_battle_options] = {"ATACK", "HEAL", "STATUS", "INVENTORY"};
        const string bool_options[n_bool_options] = {"YES", "NO"};

        void display_options(const string array[], int total) const{
            string msg = "";
            for(int i=0; i<total; i++){
                msg += to_string(i+1) + " - " +  array[i];
                if (i < total-1){
                    msg += ", ";
                }
            }
            cout << msg << endl;
        }

};

class MainCharacter: public Character{
    public:
        MainCharacter(string name, int max_hp, int attack, int defense, int level){
            NAME = name;
            MAX_HP = max_hp;
            HP = MAX_HP;
            ATTK = attack;
            DEF = defense;
            EXPEREIENCE = 0;
            NEXT_LEVEL = 10;
            LEVEL = level;
            cout << inventiry.repr();
        }

    private:
        Inventory inventiry;

};

class BattleSystem{
    public:
        BattleSystem(){
            srand(time(0));
            Character e1, e2;
            entity[PLAYER] = e1;
            entity[ENEMY] = e2;
            victory_count = 0;
            defeat_count = 0;
        }

        BattleSystem(Character& player, Character& enemy){
            srand(time(0));
            entity[PLAYER] = player;
            entity[ENEMY] = enemy;
            victory_count = 0;
            defeat_count = 0;
        }

        void player_attack(){
            cout << "PLAYER ATTACK" << endl;
            attack_entity(PLAYER, ENEMY);
        }

        void player_heal(){
            cout << "PLAYER HEALS" << endl;
            entity[PLAYER].set_hp(entity[PLAYER].get_hp() + RECUPERATE_AMOUNT);
            cout << "Heals " << RECUPERATE_AMOUNT << " points of damage" << endl;
        }

        void enemy_attack(){
            cout << "ENEMY ATTACK" << endl;
            attack_entity(ENEMY, PLAYER);
        }

        void display_characters(){
            for(int i=0; i<entity_number; i++){
                cout << entity[i].repr() << endl;
            }
        }

        void restore_characters(){
            for(int i=0; i<entity_number; i++){
                entity[i].set_hp(entity[i].get_max_hp());
            }
        }

        bool check_battle_end(){
            for(int i=0; i<entity_number; i++){
                if (entity[i].get_hp() <= 0){
                    display_characters();
                    if (i==ENEMY){
                        cout << "VICTORY" << endl;
                        victory_count++;
                        entity[PLAYER].add_exp(26);
                    }else{
                        cout << "DEFEAT" << endl;
                        defeat_count++;
                    }
                    return true;
                }
            }
            return false;
        }

        void display_vitory_count() const{
            cout << "Victories: " << victory_count << endl;
            cout << "Defeats: " << defeat_count << endl;
        }

        void display_player_stats() const{
            entity[PLAYER].display_character_stats();
        }

    private:
        const int PLAYER = 0;
        const int ENEMY = 1;
        int victory_count;
        int defeat_count;
        const static int entity_number = 2;
        int RECUPERATE_AMOUNT = 5;
        Character entity[entity_number];

        void attack_entity(int attacker_id, int defender_id){
            int extra = 1 + (rand () % 2);
            int damage = entity[attacker_id].get_attk() + extra - entity[defender_id].get_def();
            if (damage < 0){
                damage = 0;
            }
            cout << "Does " << damage << " points of damage" << endl;
            entity[defender_id].set_hp(entity[defender_id].get_hp() - damage);
        }
}; 

int main()
{
    MainCharacter player("Player", 10, 3, 1, 1);
    Character enemy("Enemy", 10, 3, 1, 1);

    BattleSystem battle(player, enemy);
    Menu menu;

    int option;

    while(true){
        // Print characters
        battle.display_characters();
        menu.display_battle_options();

        option = menu.get_option();
        if(option==menu.ATACK){
            battle.player_attack();
            battle.enemy_attack();
        }else if(option==menu.HEAL){
            battle.player_heal();
            battle.enemy_attack();
        }else if (option==menu.STATUS){
            battle.display_player_stats();
        }else if (option==menu.STATUS){
            battle.display_player_stats();
        }else{
            cout << "Command Error" << endl;
        }

        

        // Check entity HP
        if (battle.check_battle_end()){
            battle.display_vitory_count();
            cout << "Continue?" << endl;
            menu.display_bool_options();
            option = menu.get_option();
            if (option ==  menu.NO){
               break;
            }else{
                battle.restore_characters();
            }
        }
    }

    cout << "GAME OVER" << endl;
    return 0;
}
