#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

/* TODO:
    Experience and Levles
    More stats
    More clases
*/

class Character{
    public:

        Character(){
            NAME = "None";
            MAX_HP = 1;
            HP = MAX_HP;
            ATTK = 0;
            DEF = 0;
            EXPEREIENCE = 0;
            LEVEL = 1;
        }

        Character(string name, int max_hp, int attack, int defense, int level){
            NAME = name;
            MAX_HP = max_hp;
            HP = MAX_HP;
            ATTK = attack;
            DEF = defense;
            EXPEREIENCE = 0;
            LEVEL = level;
        }

        ~Character(){}
        
        int get_hp() const { return HP; }

        int get_attk() const { return ATTK; }

        int get_max_hp() const { return MAX_HP; }

        void set_hp(int value){
            if (value < 0){
                HP = 0;
            }else if (value > MAX_HP){
                HP = MAX_HP;
            }else{
                HP = value;
            }
        }

        string repr() const{
            return "<Character " + NAME + " : " + "[" + to_string(HP) + "/" + to_string(MAX_HP) + "]>";
        }

    private:

        string NAME;
        int HP, MAX_HP, ATTK, DEF; 
        int EXPEREIENCE;
        int LEVEL;


};

class BattleSystem{
    public:
        BattleSystem(){
            Character e1, e2;
            entity[PLAYER] = e1;
            entity[ENEMY] = e2;
            victory_count = 0;
            defeat_count = 0;
        }

        BattleSystem(Character& player, Character& enemy){
            entity[PLAYER] = player;
            entity[ENEMY] = enemy;
            victory_count = 0;
            defeat_count = 0;
        }

        void player_attack(){
            attack_entity(PLAYER, ENEMY);
        }

        void player_heal(){
            entity[PLAYER].set_hp(entity[PLAYER].get_hp() + RECUPERATE_AMOUNT);
        }

        void enemy_attack(){
            attack_entity(ENEMY, PLAYER);
        }

        void display_characters(){
            for(int i=0; i<entity_number; i++){
                cout << i << " - " << entity[i].repr() << endl;
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
            entity[defender_id].set_hp(entity[defender_id].get_hp() - (entity[attacker_id].get_attk() + extra) );
        }
};

class Menu{
    private:
        int ATACK = 1;
        int RECUPERATE = 2;
};

int main()
{
    int opcion;
    
    int flag_continue;
    int ATACK = 1;
    int RECUPERATE = 2;

    srand(time(0));

    Character player("Player", 10, 3, 1, 1);
    Character enemy("Enemy", 10, 3, 1, 1);

    BattleSystem battle(player, enemy);

    while(true){
        // Print characters
        battle.display_characters();

        cout << "1-ATACK, 2-RECUPERATE" << endl;
        cin >> opcion;

        if(opcion==ATACK){
            cout << "PLAYER ATTACK" << endl;
            battle.player_attack();
        }else{
            cout << "PLAYER RECUPERATES" << endl;
            battle.player_heal();
        }
        cout << "ENEMY ATTACK" << endl;
        battle.enemy_attack();

        // Check entity HP
        if (battle.check_battle_end()){
            battle.display_vitory_count();
            cout << "Continue?" << endl;
            cout << "1- YES, 2- NO" << endl;
            cin >> flag_continue;
            if (flag_continue ==  2){
               break;
            }
            battle.restore_characters();
        }
    }


    cout << "GAME OVER" << endl;
    return 0;
}
